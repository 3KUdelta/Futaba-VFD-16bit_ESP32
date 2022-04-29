# Futaba-VFD-16bit_ESP32
Driving Futaba's 16bit Vacuum Fluorescent Display with an ESP32

Author: Marc Staehli, initial upload April 2022

Features:
- Simple functions to control the display
- Includes Umlaute (äöüÄÖÜ) and the degree sign (°)
- vfd_controls.h include all functions (works similar to a library)
- settings.h as a seperate file to input your individual settings

[![Futaba-VFD-16bit-ESP32](https://github.com/3KUdelta/Futaba-VFD-16bit_ESP32/blob/main/pics/VFD_16bit.png)](https://github.com/3KUdelta/Futaba-VFD-16bit_ESP32)

[![Futaba-VFD-16bit-ESP32](https://github.com/3KUdelta/Futaba-VFD-16bit_ESP32/blob/main/pics/Futaba_VFD_16bit.jpg)](https://github.com/3KUdelta/Futaba-VFD-16bit_ESP32)

List of functions:

VFD_init(); -- must be set in setup(), initializes the display

VFD_brightness(unsigned int); -- value between 0 and 255 to set the brightness

VFD_clearScreen(); -- clears the screen

VFD_standby(bool); -- sets display in stand-by mode, value true or false

VFD_WriteASCII(int, byte); -- write a char at a specific position (e.g. VFD_WriteASCII(2, 65); writes an 'A' at position 2)

VFD_WriteStr(int, String); -- write a string at a specific position. Long strings will automatically scroll from right to left.
