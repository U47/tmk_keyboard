/*
Copyright 2025 Rony Ballouz
Sharp X1 Keyboard Protocol Implementation

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
*/

#ifndef X1_H
#define X1_H

#include <stdint.h>
#include <stdbool.h>

#if !(defined(X1_PORT) && \
      defined(X1_PIN)  && \
      defined(X1_DDR)  && \
      defined(X1_DATA_BIT))
#   error "X1 port setting is required in config.h"
#endif

// X1 Status bit definitions (active-low, sent big-endian/MSB first)
#define X1_STATUS_CTRL      (1<<0)  // CTRL (0=pressed) - was bit 7, now bit 0
#define X1_STATUS_SHIFT     (1<<1)  // SHIFT (0=pressed) - was bit 6, now bit 1  
#define X1_STATUS_KANA      (1<<2)  // KANA lock (0=locked) - was bit 5, now bit 2
#define X1_STATUS_CAPS      (1<<3)  // CAPS lock (0=locked) - was bit 4, now bit 3
#define X1_STATUS_GRAPH     (1<<4)  // GRAPH key (0=held) - was bit 3, now bit 4
#define X1_STATUS_REPEAT    (1<<5)  // Key repeat (0=repeat) - was bit 2, now bit 5
#define X1_STATUS_KEYINPUT  (1<<6)  // Key input indicator (0=input) - was bit 1, now bit 6
#define X1_STATUS_TENKEY    (1<<7)  // Ten-key press (0=pressed) - was bit 0, now bit 7

// X1 Data Structure
typedef struct {
    uint8_t status;     // First 8 bits (status indicators, active-low)
    uint8_t ascii;      // Second 8 bits (ASCII code, 0x00 = release/modifier-only)
    bool mode_b;        // True if this was from Mode B transmission
    uint32_t game_keys; // 24-bit game key state (Mode B only)
} x1_data_t;



// X1 host functions
void x1_host_init(void);
bool x1_host_recv(x1_data_t* data);

#endif