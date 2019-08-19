/*
  Done:
    - read INA219 sensor values
    - connect to WiFi network

  In progress:
  - connect to MQTT server
  - serialize readings as JSON
  - publish readings to MQTT broker
 */

#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFiNINA.h>
#include "src/MQTTManager.h"
#include "src/SensorReader.h"
#include "src/SensorReading.h"
#include "src/WiFiManager.h"

SensorReader* readers[4];

WiFiClient wifi_client;
WiFiManager wifi_manager;

IPAddress broker_ip(10, 0, 0, 175);
int broker_port = 31883;
PubSubClient* mqtt_client;
MQTTManager* mqtt_manager;

const size_t capacity = JSON_OBJECT_SIZE(5);
DynamicJsonDocument doc(capacity);
char buffer[512];

void setup(void) {

  // init and wait for serial connection
  Serial.begin(19200);
  while (!Serial) {
  }

  // start the INA219 sensors by address
  readers[0] = new SensorReader(0x40);
  readers[1] = new SensorReader(0x41);
  readers[2] = new SensorReader(0x44);
  readers[3] = new SensorReader(0x45);

  mqtt_client = new PubSubClient(wifi_client);
  mqtt_manager = new MQTTManager(mqtt_client, "arduinoMonitor");

  // init mqtt client
  mqtt_client->setServer(broker_ip, broker_port);
  mqtt_client->setCallback(MQTTManager::CallbackHandler);

  // init WiFi
  wifi_manager.Init();
  wifi_manager.ConnectWiFi();

  Serial.println("\n========================================");
  wifi_manager.PrintWiFiClientData();
  wifi_manager.UpdateCurrentNetwork();
  wifi_manager.PrintCurrentNetwork();
  Serial.println("========================================\n");

  Serial.println("Done setup...");
}

void loop(void) {
  int delayVal = 1000;
  SensorReading tmp;

  if (!mqtt_client->connected()) {
    mqtt_manager->Reconnect();
  }

  // wtf arduino...
  // https://www.arduino.cc/reference/en/language/variables/utilities/sizeof/

  for (byte i = 0; i < (sizeof(readers) / sizeof(readers[0])); i++) {
    // Serial.println(String(readers[i]->addr(), HEX));
    tmp = readers[i]->GetUpdatedReading();

    doc["addr"] = readers[i]->addr();
    doc["bus_V"] = tmp.bus_V;
    doc["shunt_mV"] = tmp.shunt_mV;
    doc["load_V"] = tmp.load_V;
    doc["current_mA"] = tmp.current_mA;
    doc["power_mW"] = tmp.power_mW;

    serializeJson(doc, Serial);    
    Serial.println();

    serializeJson(doc, buffer);
    mqtt_client->publish("outTopic", buffer);

  }

  // mqtt_client.loop();

  delay(delayVal);
}

