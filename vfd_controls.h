void write_byte(byte w_data) {
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(clk, LOW);
    if ((w_data & 0x01) == 0x01)
    {
      digitalWrite(din, HIGH);
    }
    else
    {
      digitalWrite(din, LOW);
    }
    w_data >>= 1;
    digitalWrite(clk, HIGH);
  }
  delay(1);
}
void VFD_brightness(unsigned int level) {
  digitalWrite(cs, LOW);
  write_byte(0xe4);          //byte address for setting brightness
  delayMicroseconds(5);
  write_byte(level);         //level 255 max
  digitalWrite(cs, HIGH);
  delayMicroseconds(5);
}
void VFD_clearScreen(void) {
  digitalWrite(cs, LOW);
  write_byte(0x20);
  for (int i = 0; i < 16; i++) {
    write_byte(0x20);
  }
  write_byte(0xe8);  // refresh
  digitalWrite(cs, HIGH);
  delayMicroseconds(5);
}
void VFD_show(void) {
  delay(10);
  digitalWrite(cs, LOW);
  delayMicroseconds(5);
  write_byte(0xe8);
  digitalWrite(cs, HIGH);
}
void VFD_standby(bool on) {
  digitalWrite(cs, LOW);
  if (on) {
    write_byte(0xed);     // standby mode
  }
  else {
    write_byte(0xec);     // running mode
  }
  digitalWrite(cs, HIGH);
  delayMicroseconds(5);
}
void VFD_cmd(byte command) {
  digitalWrite(cs, LOW);
  write_byte(command);
  digitalWrite(cs, HIGH);
  delayMicroseconds(5);
}
void VFD_WriteASCII(int x, unsigned char chr) {
  digitalWrite(cs, LOW);
  write_byte(0x20 + x);
  delayMicroseconds(5);
  write_byte(chr);
  digitalWrite(cs, HIGH);
  delayMicroseconds(5);
}
String translateSpecialChars(String str) {
  unsigned int l = str.length();   // how many chars were sent?
  char bla[l];                     // define working array of chars
  str.toCharArray(bla, l + 1);     // move string into array of chars
  for (int i = 0; i < l; i++) {
    if (int(bla[i]) == 195) {      // special char is coming (first is 195)
      bla[i] = 255;                // filling with marker (255 not used)
      switch (int(bla[i + 1])) {
        case 132: bla[i + 1] = 196; break; //Ä
        case 164: bla[i + 1] = 228; break; //ä
        case 150: bla[i + 1] = 214; break; //Ö
        case 182: bla[i + 1] = 246; break; //ö
        case 156: bla[i + 1] = 220; break; //Ü
        case 188: bla[i + 1] = 252; break; //ü
      }
    }
    if (int(bla[i]) == 194) {      // special char is coming (first is 194)
      bla[i] = 255;                // filling with marker (255 not used)
      bla[i + 1] = 176;            // Degree sign (°)
    }
    for (int i = 0; i < l; i++) {  // deleting marker und shifting to left
      if (int(bla[i]) == 255) {
        for (int ii = i; ii < l; ii++) {
          bla[ii] = bla[ii + 1];
        }
      }
    }
  }
  str = String(bla);
  return str;
}
void VFD_WriteStr(int x, String str) {
  str = translateSpecialChars(str);
  if (x > 0) {
    for (int i = 0; i < x; i++) {
      str = " " + str;             // fill with blanks
    }
  }
  unsigned int l = str.length();   // how many chars were sent?
  char bla[l];                     // define working array of chars
  str.toCharArray(bla, l + 1);     // move string into array of chars
  //Serial.print("Sending to display: ");
  //Serial.println(bla);

  digitalWrite(cs, LOW);
  delayMicroseconds(10);
  write_byte(0x20);                // base register DCRAM 0H

  if (l > 16) {
    for (int i = 0; i < 16 - x; i++) {
      write_byte(bla[i]);         //  fill first batch to end
    }
    digitalWrite(cs, HIGH);
    delay(1000);
    for (int i = 0; i < l - 16; i++) {
      digitalWrite(cs, LOW);
      delayMicroseconds(10);
      write_byte(0x20);                // base register DCRAM 0H
      for (int ii = 0; ii < 16; ii++) {
        write_byte(bla[ii + 1 + i]);   // shift (scroll to left)
      }
      digitalWrite(cs, HIGH);
      delay(100);
    }
  }
  else {
    for (int i = 0; i < l; i++) {
      write_byte(bla[i]);
    }
  }
  if (l <= 16) {
    for (int i = 0; i < 16 - x - l; i++) {
      write_byte(0x20);            // fill with blanks
    }
  }
  digitalWrite(cs, HIGH);
}
void VFD_init() {
  /* Setting Digits ****************************************************/
  digitalWrite(cs, LOW);
  write_byte(0xe0);            //first byte address for setting digits
  delayMicroseconds(5);
  write_byte(0x0f);            //16 digits equals value 15 (hex 0x0f)
  digitalWrite(cs, HIGH);
  delayMicroseconds(5);
  /* *******************************************************************/
  VFD_brightness(brightness);  //setting brightness to initial values
  VFD_standby(false);          //switch standby off
  VFD_clearScreen();           //clear the screen
  VFD_show();                  //activate the screen
  /* Quick test of all digits ******************************************/
  for (int i = 10; i-- > 0;) {
    for (int ii = 0; ii < 16; ii++) {
      VFD_WriteASCII(ii, i + 0x30);
      delay(10);
    }
  }
  /* *******************************************************************/
  VFD_clearScreen();           //clear the screen
  VFD_WriteStr(2, ver);        //write version number to screen
  delay(1000);
}
