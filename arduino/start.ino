#include <Wire.h>
#include <Adafruit_INA219.h>
#include <ArduinoMqttClient.h>
#include <MqttClient.h>

Adafruit_INA219 sensor219_0x40;
Adafruit_INA219 sensor219_0x41(0x41);
Adafruit_INA219 sensor219_0x44(0x44);
Adafruit_INA219 sensor219_0x45(0x45);
Adafruit_INA219 sensors[4] = { sensor219_0x40, sensor219_0x41, sensor219_0x44, sensor219_0x45 };

void setup(void){

  Serial.begin(9600);
  sensor219_0x40.begin();
  sensor219_0x41.begin();
  sensor219_0x44.begin();
  sensor219_0x45.begin();
  
}

void printSensorReadings(Adafruit_INA219 sensor) {
  
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = sensor.getShuntVoltage_mV();
  busvoltage = sensor.getBusVoltage_V();
  current_mA = sensor.getCurrent_mA();
  power_mW = sensor.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");
  
}

void printSensorArrayValues(Adafruit_INA219 sensors[]){
  //Serial.println("here");
  for(byte i = 0; i < sizeof(sensors); i++) {
    //Serial.println(i);
    printSensorReadings(sensors[i]);    
  }    
}
void loop(void){
  int delayVal = 1000;

  Serial.println("0x40");
  printSensorReadings(sensor219_0x40);
  
  Serial.println("0x41");  
  printSensorReadings(sensor219_0x41);

  Serial.println("0x44");
  printSensorReadings(sensor219_0x44);

  Serial.println("0x45");
  printSensorReadings(sensor219_0x45);
  
  //printSensorArrayValues(sensors);
  //Serial.println(sizeof(sensors));
  
  delay(delayVal);

}