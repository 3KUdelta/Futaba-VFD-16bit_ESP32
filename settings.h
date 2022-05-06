/**************************************************************************
  Settings for VFD Futuba - Type No. 8-MD-06INKM
  SPI connection on MOSI, CLK and CS
  SPI according to datasheet: 0.5 MhZ, clock high idle, data on rising, LBSfirst

  8 or 16 bits (digits) display
  Brigthness 0 - 255

  MIT License

  Copyright (c) 2022 Marc Stähli

  https://github.com/3KUdelta/Futaba-VFD-16bit_ESP32

 ***************************************************************************/
String ver = "Version 1.2";
unsigned int digits = 16;      // either 8 or 16 digits for the Futaba VFD
unsigned int brightness = 50;  // initial brightness settings
