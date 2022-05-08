# Futaba-VFD-16bit_ESP32
**Driving Futaba's 16bit Vacuum Fluorescent Display with an ESP32**

Type of display: **8-MD-06INKM**

Author: Marc Staehli, initial upload April 2022

Fully refactored code of what was originally delivered with the display. Using SPI.h library for reliable control.

Features:
- Works with 8 digits and with 16 digits (change in settings.h)
- Simple functions to control the display
- Includes Umlaute (äöüÄÖÜ) and the degree sign (°)
- vfd_controls.h includes all functions (works similar as a library)
- settings.h defined as a seperate file to input your individual settings

[![Futaba-VFD-16bit-ESP32](https://github.com/3KUdelta/Futaba-VFD-16bit_ESP32/blob/main/pics/VFD_16bit.png)](https://github.com/3KUdelta/Futaba-VFD-16bit_ESP32)

this one was bought here: https://www.aliexpress.com/item/1005001498957894.html


[![Futaba-VFD-16bit-ESP32](https://github.com/3KUdelta/Futaba-VFD-16bit_ESP32/blob/main/pics/Futaba_VFD_16bit.jpg)](https://github.com/3KUdelta/Futaba-VFD-16bit_ESP32)

Connections display to ESP32:

- din   = 23; // DA (MOSI)
- clk   = 18; // CK (CLK)
- cs    = 5;  // CS (SS)

the 8bit display needs additionally:

- en    = solder blob to shorten with +5V
- reset = must be set high (3.3V) to work - ground to be resetted


List of functions:

- **VFD_init();** -- must be set in setup(), initializes the display

- **VFD_brightness(unsigned int);** -- value between 0 and 255 to set the brightness

- **VFD_clearScreen();** -- clears the screen

- **VFD_standby(bool);** -- sets display in stand-by mode, value true or false

- **VFD_WriteASCII(int, byte);** -- write a char at a specific position (e.g. VFD_WriteASCII(2, 65); writes an 'A' at position 3. (0 is first position))

- **VFD_WriteStr(int, String);** -- write a string at a specific position. Long strings will automatically scroll from right to left.
