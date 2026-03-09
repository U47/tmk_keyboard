/*
Copyright 2025 Rony Ballouz
Sharp X1 Keyboard Protocol Implementation

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
*/

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "x1.h"
#include "print.h"

// Pin access macros
#define x1_data_in() (X1_PIN & (1<<X1_DATA_BIT))

// Protocol timing (in Timer1 ticks at 16MHz with /8 prescaler = 0.5µs per tick)
#define TIMER_TICKS_PER_US      2   // 16MHz / 8 / 1000000 = 2 ticks per µs

// Timing thresholds for edge detection (±25µs tolerance)
#define HEADER_MODE_A_MIN       (950 * TIMER_TICKS_PER_US)     // 950µs (1000-50)
#define HEADER_MODE_A_MAX      (1050 * TIMER_TICKS_PER_US)     // 1050µs (1000+50)
#define HEADER_MODE_A_HIGH_MIN  (675 * TIMER_TICKS_PER_US)     // 675µs (200-25)
#define HEADER_MODE_A_HIGH_MAX  (725 * TIMER_TICKS_PER_US)     // 725µs (200+25)

#define HEADER_MODE_B_MIN       (375 * TIMER_TICKS_PER_US)     // 375µs (400-25)
#define HEADER_MODE_B_MAX       (425 * TIMER_TICKS_PER_US)     // 425µs (400+25)
#define HEADER_MODE_B_HIGH_MIN  (175 * TIMER_TICKS_PER_US)     // 200µs (200-25)
#define HEADER_MODE_B_HIGH_MAX  (225 * TIMER_TICKS_PER_US)     // 225µs (200+25)

#define BIT_LOW_MIN             (225 * TIMER_TICKS_PER_US)     // 225µs (250-25)
#define BIT_LOW_MAX             (275 * TIMER_TICKS_PER_US)     // 275µs (250+25)

// Mode A bit high thresholds (±25µs tolerance)
#define MODE_A_BIT_HIGH_SHORT_MIN  (725 * TIMER_TICKS_PER_US)  // 725µs (750-25)
#define MODE_A_BIT_HIGH_SHORT_MAX  (775 * TIMER_TICKS_PER_US)  // 775µs (750+25)
#define MODE_A_BIT_HIGH_LONG_MIN  (1700 * TIMER_TICKS_PER_US)  // 1700µs (1750-50)
#define MODE_A_BIT_HIGH_LONG_MAX  (1800 * TIMER_TICKS_PER_US)  // 1800µs (1750+50)

// Mode B bit high thresholds (±25µs tolerance)
#define MODE_B_BIT_HIGH_SHORT_MIN (225 * TIMER_TICKS_PER_US)   // 225µs (250-25)
#define MODE_B_BIT_HIGH_SHORT_MAX (275 * TIMER_TICKS_PER_US)   // 275µs (250+25)
#define MODE_B_BIT_HIGH_LONG_MIN  (725 * TIMER_TICKS_PER_US)   // 700µs (750-25)
#define MODE_B_BIT_HIGH_LONG_MAX  (775 * TIMER_TICKS_PER_US)   // 800µs (750+25)

// Protocol states
typedef enum {
    X1_STATE_IDLE,              // Waiting for transmission
    X1_STATE_RECEIVING_A,       // Receiving Mode A data (16 bits)
    X1_STATE_RECEIVING_B        // Receiving Mode B data (24 bits)
} x1_protocol_state_t;

// Global state  
static volatile x1_protocol_state_t protocol_state = X1_STATE_IDLE;
static volatile uint8_t bit_count = 0;
static volatile uint32_t rx_data = 0;
static volatile x1_data_t rx_buffer = {0, 0, false, 0};
static volatile bool data_ready = false;

void x1_host_init(void)
{
    // Configure pin as input with pullup
    X1_DDR &= ~(1<<X1_DATA_BIT);
    X1_PORT |= (1<<X1_DATA_BIT);
    
    // Configure Timer1 for interval measurements
    // Prescaler /8 (0.5µs per tick at 16MHz, 2 ticks per µs)
    TCCR1B = (1<<CS11);
    
    // Configure INT0 for falling edge 
    EICRA |= (1<<ISC01);        
    EICRA &= ~(1<<ISC00);

    EIMSK |= (1<<INT0);         // Enable INT0
    
    protocol_state = X1_STATE_IDLE;
    sei();
}

bool x1_host_recv(x1_data_t* data)
{
    if (!data_ready) return false;
    
    cli();
    *data = rx_buffer;
    data_ready = false;
    sei();
    
    return true;
}

// Reset protocol state to idle
inline
static void reset_to_idle(void)
{
    protocol_state = X1_STATE_IDLE;
    bit_count = 0;
    rx_data = 0;
    
    // Set INT0 for falling edge detection
    EICRA &= ~(1<<ISC00);
}

// External interrupt INT0 - measures time between pulses to determine bit 
ISR(INT0_vect)
{
    uint16_t duration = TCNT1;  // Time since last edge
    TCNT1 = 0;  // Reset timer for next interval measurement
    
    if (EICRA & (1<<ISC00)) {
        // Rising edge occurred - measure the low time that just ended
        switch (protocol_state) {
            case X1_STATE_IDLE:
                // Validate header
                if (duration >= HEADER_MODE_A_MIN && duration <= HEADER_MODE_A_MAX) {
                    protocol_state = X1_STATE_RECEIVING_A;
                    bit_count = 0;
                    rx_data = 0;
                    rx_buffer.mode_b = false;
                } else if (duration >= HEADER_MODE_B_MIN && duration <= HEADER_MODE_B_MAX) {
                    protocol_state = X1_STATE_RECEIVING_B;
                    bit_count = 0;
                    rx_data = 0;
                    rx_buffer.mode_b = true;
                }else{
                    // Invalid header timing - reset to idle
                    reset_to_idle();
                    return;
                }
                break;
                
            case X1_STATE_RECEIVING_A:
                if (duration >= BIT_LOW_MIN && duration <= BIT_LOW_MAX) {
                    //if this was a stop bit then send the packet
                    if(bit_count>=16){
                        // Valid 16-bit Mode A transmission complete
                        rx_buffer.status = (uint8_t)((rx_data >> 8) & 0xFFUL);
                        rx_buffer.ascii = (uint8_t)(rx_data & 0xFFUL);
                        rx_buffer.game_keys = 0;  // Mode A has no game keys
                        data_ready = true;
                        reset_to_idle();
                        return;
                    }
                // Pass if this was a valid bit low period (250µs)
                } else {
                    // Invalid bit timing - reset to idle
                    reset_to_idle();
                    return;
                }
                break;
            case X1_STATE_RECEIVING_B:
                if (duration >= BIT_LOW_MIN && duration <= BIT_LOW_MAX) {
                    //if this was a stop bit then send the packet
                    if (bit_count >= 24) {
                        rx_buffer.status = 0xFF;  // No status in Mode B packet
                        rx_buffer.ascii = 0x00;   // No ASCII in Mode B packet
                        rx_buffer.game_keys = rx_data;
                        data_ready = true;  // Send Mode B packet to matrix.c
                        reset_to_idle();
                        return;
                    }
                } else {
                    // Invalid bit timing - reset to idle
                    reset_to_idle();
                    return;
                }
                break;
        }
        
        // Switch to falling edge detection for next transition
        EICRA &= ~(1<<ISC00);
        
    } else {
        // Falling edge occurred - measure the high time that just ended
        
        switch (protocol_state) {
            case X1_STATE_IDLE:
                // Falling edge in idle is the start of header - reset timer and switch to rising edge
                // Switch to rising edge detection to measure header low time
                EICRA |= (1<<ISC01) | (1<<ISC00);
                return;
                
            case X1_STATE_RECEIVING_A:
                // Decode Mode A bit based on high time
                if (duration >= MODE_A_BIT_HIGH_SHORT_MIN && duration <= MODE_A_BIT_HIGH_SHORT_MAX) {
                    rx_data = (rx_data << 1) | 0;
                    bit_count++;
                } else if (duration >= MODE_A_BIT_HIGH_LONG_MIN && duration <= MODE_A_BIT_HIGH_LONG_MAX) {
                    rx_data = (rx_data << 1) | 1;
                    bit_count++;
                } else if (bit_count == 0 && duration >= HEADER_MODE_A_HIGH_MIN && duration <= HEADER_MODE_A_HIGH_MAX) {
                    // pass through header high time 
                } else {
                    // Invalid timing - reset
                    reset_to_idle();
                    return;
                }
                break;
                
            case X1_STATE_RECEIVING_B:
                // Decode Mode B bit based on high time  
                if (duration >= MODE_B_BIT_HIGH_SHORT_MIN && duration <= MODE_B_BIT_HIGH_SHORT_MAX) {
                    rx_data = (rx_data << 1) | 0;
                    bit_count++;
                } else if (duration >= MODE_B_BIT_HIGH_LONG_MIN && duration <= MODE_B_BIT_HIGH_LONG_MAX) {
                    rx_data = (rx_data << 1) | 1;
                    bit_count++;
                } else if (bit_count==0 && duration >= HEADER_MODE_B_HIGH_MIN && duration < HEADER_MODE_B_HIGH_MAX){
                    // pass through header high time 
                }else {
                    // Invalid timing - reset
                    reset_to_idle();
                    return;
                }
                
                break;
        }
        
        // Switch to rising edge detection for next transition
        EICRA |= (1<<ISC01) | (1<<ISC00);
    }
    
}