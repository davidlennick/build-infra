#include "SensorReader.h"

// public

SensorReader::SensorReader(uint8_t addr) {
  this->addr_ = addr;
  this->sensor_ = Adafruit_INA219(this->addr_);
  this->sensor_.begin();
  this->reading_ = SensorReading();
}

SensorReader::SensorReader(uint8_t addr, Adafruit_INA219 sensor) {
  this->addr_ = addr;
  this->sensor_ = sensor;
  this->sensor_.begin();
  this->reading_ = SensorReading();
}

Adafruit_INA219 SensorReader::sensor() { return this->sensor_; }

SensorReading SensorReader::reading() { return this->reading_; }

uint8_t SensorReader::addr() { return this->addr_; }

// String SensorReader::GetAddrString() {
  
//   //itoa(this->addr, buffer, 10);
// }

void SensorReader::UpdateReading() {
  this->reading_.shunt_V = this->sensor_.getShuntVoltage_mV();
  this->reading_.bus_V = this->sensor_.getBusVoltage_V();
  this->reading_.current_mA = this->sensor_.getCurrent_mA();
  this->reading_.power_mW = this->sensor_.getPower_mW();
  this->reading_.load_V = this->reading_.bus_V + (this->reading_.shunt_V / 1000);
}

SensorReading SensorReader::GetUpdatedReading() {
  this->UpdateReading();
  return this->reading();
}

void SensorReader::PrintCurrentReading() {
  Serial.print("Bus Voltage:   ");
  Serial.print(this->reading_.bus_V);
  Serial.println(" V");
  Serial.print("Shunt Voltage: ");
  Serial.print(this->reading_.shunt_V);
  Serial.println(" mV");
  Serial.print("Load Voltage:  ");
  Serial.print(this->reading_.load_V);
  Serial.println(" V");
  Serial.print("Current:       ");
  Serial.print(this->reading_.current_mA);
  Serial.println(" mA");
  Serial.print("Power:         ");
  Serial.print(this->reading_.power_mW);
  Serial.println(" mW");
  Serial.println("");
}

void SensorReader::PrintUpdatedReading() {
  this->UpdateReading();
  this->PrintCurrentReading();
}