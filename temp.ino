#include <OneWire.h>
#include <DallasTemperature.h>
#include "SevSeg.h"

// pin for communicating to the DS18B20 via 1-Wire bus
const int ONE_WIRE_BUS = A5;
const int speakerPin = A0;
// Setup 1-Wire instance to commicate with DS18B20
OneWire oneWireLocal(ONE_WIRE_BUS);
// Pass oneWire reference to DallasTemperature
DallasTemperature sensorsLocal(&oneWireLocal);

SevSeg sevseg; // Initiate object

float tempLocal = 0.0;

void setup()
{
  Serial.begin(9600);
  sensorsLocal.begin();
  pinMode(speakerPin, OUTPUT);

  byte numDigits = 4;
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(90);
}

void loop()
{
 sevseg.blank();
 CheckTemps();
 Serial.print("Current Temp: ");
 Serial.println(tempLocal);
 sevseg.setNumber(tempLocal, 2);

 if (tempLocal > 28.0) {
   for (int i = 0; i < 3; i++) {
     analogWrite(speakerPin, 255);
     delay (60);
     analogWrite (speakerPin, 0);
     delay (30);
   }
 } else {
   analogWrite(speakerPin, 0);
 }
 for (int i = 0; i < 2000; i++) {
   sevseg.refreshDisplay();
   delay(1);
 }
}

void CheckTemps()
{
  sensorsLocal.requestTemperatures();
  delay(100);
  tempLocal = sensorsLocal.getTempCByIndex(0);
}
