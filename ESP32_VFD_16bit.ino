#include "settings.h"
#include "vfd_controls.h"

void setup() {

  Serial.begin(115200); while (!Serial); delay(200);
  Serial.println();
  Serial.println("Starting VFD...");
  Serial.println();

  pinMode(clk, OUTPUT);
  pinMode(din, OUTPUT);
  pinMode(cs, OUTPUT);

  VFD_init();            //initializing the 16bit display

  VFD_WriteStr(0, "Welcome!");
  delay(2000);
}

void loop()
{
  VFD_WriteStr(16, "Writing a very long String to see scrolling.");
  delay(1000);

  VFD_clearScreen();
  delay(3000);
}
