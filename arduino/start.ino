/*
  Done:
    - read INA219 sensor values
    - connect to WiFi network

  In progress:
  - connect to MQTT server
  - serialize readings as JSON
  - publish readings to MQTT broker
 */

#include <WiFiNINA.h>
#include "src/SensorReader.h"
#include "src/SensorReading.h"
#include "src/WiFiManager.h"


WiFiManager wifi_manager;
SensorReader *readers[4];

void setup(void) {
  
  // init and wait for serial connection
  Serial.begin(9600);  
  while (!Serial) { } 

  // start the INA219 sensors by address
  readers[0] = new SensorReader(0x40);
  readers[1] = new SensorReader(0x41);
  readers[2] = new SensorReader(0x44);
  readers[3] = new SensorReader(0x45);

  // initialize WiFi 
  wifi_manager.Init();
  wifi_manager.ConnectWiFi();

  Serial.println("Done setup...");
  
}

void loop(void) {
  int delayVal = 5000;

  // wtf arduino...
  // https://www.arduino.cc/reference/en/language/variables/utilities/sizeof/
  
  // for (byte i = 0; i < (sizeof(readers) / sizeof(readers[0])); i++) {
  //   Serial.println(String(readers[i]->addr(), HEX));
  //   readers[i]->PrintUpdatedReading();
  // }

  wifi_manager.UpdateCurrentNetwork();
  wifi_manager.PrintCurrentNetwork();
  wifi_manager.PrintWiFiClientData();

  Serial.println("Here");
  delay(delayVal);
}
