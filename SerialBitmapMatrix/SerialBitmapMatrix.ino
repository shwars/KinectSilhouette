#include <RTClib.h>
#include <Wire.h>
#include <stdint.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  InitLEDMatrix();
}

void displayByte(uint8_t data,short byteNumber){
  short xOffset=byteNumber%2*8;
  short yOffset=byteNumber/2;
  for(int i=7;i>=0;--i) SetPixel(xOffset+7-i,yOffset,(data&(1<<i))>>i);
}

void loop() {
  LEDloop();
  //displayByte(0b10100111,5);
  if(Serial.available()==32)
    for(int i=0;i<32;++i)
      displayByte(Serial.read(),i);
}

