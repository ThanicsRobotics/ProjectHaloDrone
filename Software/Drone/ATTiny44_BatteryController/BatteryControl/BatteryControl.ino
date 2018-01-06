#include "TinyWire.h"
#define addr 0x04

#define pwrControl 10
#define tempPin A7
#define sensePin 9
#define cellPin1 A0
#define cellPin2 A1
#define cellPin3 A2
#define cellPin4 A3

//Analog ADC vales
unsigned long aCellValue1 = 0;
unsigned long aCellValue2 = 0;
unsigned long aCellValue3 = 0;
unsigned long aCellValue4 = 0;

//Actual cell voltages
float Cell1 = 0.000;  
float Cell2 = 0.000;
float Cell3 = 0.000;
float Cell4 = 0.000;

float adcVoltage = 0.0048; //one point on the ADC equals this many volts

float temp = 0;

void setup() {
  //Latch power
  pinMode(pwrControl, OUTPUT);
  digitalWrite(pwrControl, HIGH);

  //Initialize comms
  TinyWire.begin(addr);
  TinyWire.onRequest(onI2CRrequest);
  //mySerial.begin(9600);
}

void loop() {
  //If button pressed
  if (digitalRead(sensePin) == HIGH) {
    handleInterrupt();
  }
  getTemp();
  //getCellVoltages();
}

void onI2CRrequest() {
  TinyWire.send(temp);
}

void getTemp() {
  //Temperature Reading
  int tempOutput = analogRead(tempPin);
  temp = ((tempOutput * adcVoltage) - 0.5) / 0.01;    //Temp sensor's Vout = Tc x Ta + V0, Tc = 10mV/C, V0 = 500mV
}

//void getCellVoltages() {
//  //reading cell voltages in analog pins
//  int iterations = 2000;
//  
//  for(int i=0;i < iterations;i++){
//    aCellValue1 = aCellValue1 + analogRead(cellPin1);
//    aCellValue2 = aCellValue2 + analogRead(cellPin2);
//    aCellValue3 = aCellValue3 + analogRead(cellPin3);
//    aCellValue4 = aCellValue4 + analogRead(cellPin4);
//  }
//   
//  aCellValue1 = aCellValue1 / iterations;
//  aCellValue2 = aCellValue2 / iterations;
//  aCellValue3 = aCellValue3 / iterations;
//  aCellValue4 = aCellValue4 / iterations;
//
//  //convert analog values to voltages
//  Cell1 = (aCellValue1 * adcVoltage);
//  Cell2 = (aCellValue2 * adcVoltage * 2) - Cell1;
//  Cell3 = (aCellValue3 * adcVoltage * 3) - Cell2 - Cell1;
//  Cell4 = (aCellValue4 * adcVoltage * 4) - Cell3 - Cell2 - Cell1;
//
//}

void handleInterrupt() {
  //Wait for 1 sec button hold
  delay(1000);
  if (digitalRead(sensePin) == HIGH) {
    //Turn off power MOSFET, shut down
    digitalWrite(pwrControl, LOW);
  }
}










