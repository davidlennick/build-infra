#include <string.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
//#include <WiFiNINA.h>
#include <Ethernet.h>
#include <Wire.h>
#include "src/MQTTManager.h"
#include "src/SensorReader.h"
#include "src/SensorReading.h"
#include "src/WiFiManager.h"


// const config vars
const uint8_t SENSOR_ADDRS[] = { 0x40, 0x41, 0x44, 0x45 };
const byte ETH_MAC[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
const IPAddress BROKER_IP(10, 0, 0, 175);
const int BROKER_PORT = 31883;
const String BASE_TOPIC = "outTopic";
const int DELAY_VAL = 200;

// ina219 sensor vars
SensorReader* readers;
SensorReading reading;

// wifi vars (not used)
WiFiClient* wifi_client;
WiFiManager* wifi_manager;

// ethernet vars 


// mqtt vars
PubSubClient* mqtt_client;
MQTTManager* mqtt_manager;

// json vars
const size_t capacity = JSON_OBJECT_SIZE(6);
DynamicJsonDocument doc(capacity);
char json_buffer[512];

// debug vars

int crash_count = 0;
int msg_count = 0;


void setup(void) {
  // init and wait for serial connection
  Serial.begin(19200);
  while (!Serial) {
  }

  Serial.println("========================================");
  Serial.println("Starting setup...");
  Serial.println("========================================");
  Serial.println();

  InitSensorReaders();
  //InitWiFi();
  InitEth();
  InitMQTT();
  delay(100);

  Serial.println();
  Serial.println("========================================");
  Serial.println("Done setup");
  Serial.println("========================================");
}

void loop(void) {
  String base_topic = BASE_TOPIC + "/";
  String addr;
  String full_topic;

  CheckConn();

  // https://www.arduino.cc/reference/en/language/variables/utilities/sizeof/

  for (byte i = 0; i < (sizeof(readers) / sizeof(readers[0])); i++) {
    //reading = readers[i]->GetUpdatedReading();
    //addr = readers[i]->GetAddrAsString();
    
    // create the topic to publish to
    //full_topic = base_topic + addr;

    // // convert sensor readings to json and print to console
    //SensorReadingToJson(addr, reading, doc);
    //PrintReadingJson(doc, full_topic);

    // // store the json doc in a buffer
    //serializeJson(doc, json_buffer);
    //mqtt_client->publish(full_topic, json_buffer);
    mqtt_client->publish("outTopic", "{}");
    msg_count++;

    delay(100);
  }
  Serial.print("Free RAM: ");
  Serial.println(FreeRAM());
  //delay(500);

}

void InitSensorReaders() {
  // start the INA219 sensors by address
  int num_sensors = sizeof(SENSOR_ADDRS) / sizeof(uint8_t);
  readers = SensorReader[num_sensors];

  for (int i = 0; i < num_sensors; i++) {
    readers[i] = new SensorReader(SENSOR_ADDRS[i]);
  }

  //readers[0] = new SensorReader(0x40);
  //readers[1] = new SensorReader(0x41);
  //readers[2] = new SensorReader(0x44);
  //readers[3] = new SensorReader(0x45);
}

void InitEth() {

}

void InitWiFi() {
  // init WiFi
  wifi_client = new WiFiClient();
  wifi_manager = new WiFiManager(wifi_client);

  wifi_manager->Init();
  wifi_manager->ConnectWiFi();

  Serial.println("\n========================================");
  wifi_manager->PrintWiFiClientData();
  wifi_manager->UpdateCurrentNetwork();
  wifi_manager->PrintCurrentNetwork();
  Serial.println("========================================\n");
}

void InitMQTT() {
  // init mqtt client
  mqtt_client = new PubSubClient(*wifi_client);
  mqtt_manager = new MQTTManager(mqtt_client, "arduinoMonitor");

  mqtt_client->setServer(BROKER_IP, BROKER_PORT);
  mqtt_client->setCallback(MQTTManager::CallbackHandler);
  mqtt_manager->Reconnect();
}

void CheckConn() {
  
  if (!wifi_manager->IsWiFiConnected()) {
    crash_count++;
    Serial.print("Lost Wifi connection. Status: ");    
    Serial.println(wifi_manager->wifi()->status());
    Serial.print("Crash count: ");
    Serial.println(crash_count);
    Serial.print("Message count: ");
    Serial.println(msg_count);
    Serial.println("Attempting reconnect...");
    FreeRAM();
    if (!wifi_manager->ConnectWiFi()) {
      Serial.println();
      Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      Serial.print("Failed to reconnect. Please check that ");
      Serial.print(SECRET_SSID);
      Serial.println(" is in range and restart device.");
      Serial.print("Crash count: ");
      Serial.println(crash_count);
      Serial.print("Message count: ");
      Serial.println(msg_count);
      Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      while (true) {}
    }
    else {
      Serial.println("\n========================================");
      wifi_manager->PrintWiFiClientData();
      wifi_manager->UpdateCurrentNetwork();
      wifi_manager->PrintCurrentNetwork();
      Serial.println("========================================\n"); 
      delay(500);   
      mqtt_manager->Reconnect();
    }
  }

  if (!mqtt_client->connected()) {
    Serial.println("Lost MQTT connection, attempting reconnect...");
    mqtt_manager->Reconnect();
  }
}

void SensorReadingToJson(String addr, SensorReading reading, DynamicJsonDocument& doc) {
  doc["addr"] = addr.toInt();
  doc["bus_V"] = reading.bus_V;
  doc["shunt_mV"] = reading.shunt_mV;
  doc["load_V"] = reading.load_V;
  doc["current_mA"] = reading.current_mA;
  doc["power_mW"] = reading.power_mW;
}

void PrintReadingJson(DynamicJsonDocument doc, String topic) {
  Serial.print("[");
  Serial.print(topic);
  Serial.print("] ");
  serializeJson(doc, Serial);
  Serial.println();
}

// https://forum.arduino.cc/index.php?topic=215334.msg1576106#msg1576106
int FreeRAM() {
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

// https://coderwall.com/p/zfmwsg/arduino-string-to-char
char* StringToChar(String command) {
  if (command.length() != 0) {
    char* p = const_cast<char*>(command.c_str());
    return p;
  }
}