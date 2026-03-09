X1 to USB keyboard converter
============================
Can connect the Sharp X1 (turbo) keyboard over USB.

The X1 was a Z80-based computer released in Japan from 1982 to 1987.  
https://en.wikipedia.org/wiki/Sharp_X1


Info
----
Wiki:  
https://github.com/tmk/tmk_keyboard/wiki

X1 protocol:  
http://www.x1center.org/resource/x1kbd.pdf  
https://kyouichisato.blogspot.com/2014/07/sharp-x1-ps2.html  
https://deskthority.net/viewtopic.php?t=26795  

Firmware Code:  
https://github.com/tmk/tmk_keyboard/tree/master/converter/x1_usb



Wiring
------
Connect DATA, VCC and GND to microcontroller. Use port **PD0** for DATA.

You can change the port with `X1_PORT`, `X1_PIN`, `X1_DDR`, `X1_DATA_BIT` in `config.h`.

X1 TRS plug:

    ────┐
        ├─────────╥╥────╥╥─⌄─. 
        │     S   ║║ R  ║║  T :
        ├─────────╨╨────╨╨─⌃─'
    ────┘     │      │      │
              │      │      └── T (VCC)
              │      └───────── R (Data)
              └──────────────── S (GND)


### Pull-up resistor:
Don't use more than 10kΩ as an external pull-up. The keyboard's data line has ~1kΩ output resistance.



Build firmware and Program microcontroller
------------------------------------------
To build firmware and program TMK X1 Converter run these commands:

    $ make -f Makefile clean
    $ make -f Makefile [KEYMAP=yourname]
    $ make -f Makefile [KEYMAP=yourname] dfu

You can select keymap name with optional `KEYMAP=` ('plain' is default name). Push button on the converter before running `dfu` target.

https://github.com/tmk/tmk_keyboard/wiki#build-firmware



Debug
-----
Use hid_listen command line tool to see debug outputs.

https://github.com/tmk/tmk_keyboard/wiki#debug



Notes for keyboard
------------------
This keyboard doesn't send scancodes like a normal keyboard -- it sends the ASCII code, which creates lots of collisions.
A few keypress combinations cannot be told apart digitally, and some of those combinations will get ignored or default to the most likely use-case.


Locking CapsLock
----------------

The converter supports the locking Capslock key. Use keycode `LCAP` instead of `CAPS` in your keymap.  
The Kana key is also locking but currently not supported in tmk_core.
