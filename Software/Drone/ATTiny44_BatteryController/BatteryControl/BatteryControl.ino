//#include <USIWire.h>
//#include "TinyWire.h"
#include <SoftwareSerial.h>
SoftwareSerial mySerial(1,10);
//#define addr 0x03
//#define pwrControl 10
#define tempPin A7

void setup() {
//  pinMode(pwrControl, OUTPUT);
//  digitalWrite(pwrControl, HIGH);
//  TinyWire.begin(addr);
  mySerial.begin(9600);
}

void loop() {
  int tempOutput = analogRead(tempPin);
  float temp = ((tempOutput * 0.0048828125) - 0.5) / 0.01;                      //Temp sensor's Vout = Tc x Ta + V0, Tc = 10mV/C, V0 = 500mV
  mySerial.println(temp);
  delay(1000);
}
