#include <Adafruit_INA219.h>
#include <ArduinoMqttClient.h>
#include <MqttClient.h>
#include <Wire.h>
#include "src/SensorReader.h"
#include "src/SensorReading.h"

SensorReader *readers[4];

void setup(void) {
  Serial.begin(9600);
  readers[0] = new SensorReader(0x40);
  readers[1] = new SensorReader(0x41);
  readers[2] = new SensorReader(0x44);
  readers[3] = new SensorReader(0x45);
}

void loop(void) {
  int delayVal = 1000;

  for (byte i = 0; i < sizeof(readers); i++) {
    Serial.println(String(readers[i]->addr(), HEX));
    readers[i]->PrintUpdatedReading();
  }

  delay(delayVal);
}