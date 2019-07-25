#ifndef SENSORREADER_H_
#define SENSORREADER_H_

#include "Adafruit_INA219.h"
#include "Arduino.h"
#include "SensorReading.h"

class SensorReader {
 public:
  SensorReader();
  SensorReader(uint8_t addr);
  SensorReader(uint8_t addr, Adafruit_INA219 sensor);
  Adafruit_INA219 sensor();
  SensorReading reading();
  uint8_t addr();
  String GetAddrString();
  SensorReading GetUpdatedReading();
  void UpdateReading();
  void PrintCurrentReading();
  void PrintUpdatedReading();

 private:
  uint8_t addr_;
  Adafruit_INA219 sensor_;
  SensorReading reading_;
};

#endif