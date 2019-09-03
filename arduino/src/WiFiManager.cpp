#include "WiFiNINA.h"
#include "WiFiManager.h"

// https://www.arduino.cc/en/Reference/WiFiNINA

// wrapper/facade class to shorten some of the wifi library stuff

WiFiManager::WiFiManager(WiFiClient* client) {
  this->wifi_ = new WiFiClass(); 
  this->client_ = client; 
}

WiFiNetwork WiFiManager::current_network() { return this->network_; }

WiFiClass* WiFiManager::wifi() {
  return this->wifi_;
}

void WiFiManager::Init() {
  if (this->wifi_->status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true) {
    }
  }

  if (this->wifi_->firmwareVersion() != WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade firmware");
    Serial.println(this->wifi_->firmwareVersion());
    Serial.println(WIFI_FIRMWARE_LATEST_VERSION);
  }
}

bool WiFiManager::ConnectWiFi(byte attempt_limit=5) {
  // attempt to connect to WiFi network:
  byte counter = 0;
  bool conn_stat = this->IsWiFiConnected();

  while (conn_stat == false && counter < attempt_limit) {
    Serial.println();
    Serial.print("Attempting to connect to WPA SSID ");
    Serial.print(SECRET_SSID);
    Serial.println();
    this->wifi_->disconnect();
    // Connect to WPA/WPA2 network:
    this->wifi_->begin(SECRET_SSID, SECRET_PASS);

    // allow dropout of loop on WL_CONNECT
    for (byte i = 0; i < 10; i++) {
      
      conn_stat = this->IsWiFiConnected();

      if (!conn_stat) {
        Serial.print(".");
        delay(1000);
      } 
      else {
        return conn_stat;
      }     
    }

    counter++;
  }
       
  return this->IsWiFiConnected();

}

bool WiFiManager::IsWiFiConnected() {
  Serial.print("Checking WiFi connection status: ");
  Serial.print(this->wifi_->status());
  if (this->wifi_->status() == WL_CONNECTED) {
    Serial.println(" true");
    return true;
  }
  else {
    Serial.println(" false");
  }
  return false;
}

void WiFiManager::UpdateCurrentNetwork() {
  this->network_.ssid = this->wifi_->SSID();
  this->network_.rssi = this->wifi_->RSSI();
  this->wifi_->BSSID(this->network_.bssid);
  this->network_.encryption = WiFi.encryptionType();
}

void WiFiManager::PrintWiFiClientData() {
  // print your board's IP address:
  byte mac[6];

  Serial.print("IP Address: ");
  Serial.println(this->wifi_->localIP());

  Serial.print("MAC address: ");
  WiFiManager::PrintMACAddress(this->wifi_->macAddress(mac));
}

void WiFiManager::PrintCurrentNetwork() {
  this->UpdateCurrentNetwork();
  WiFiManager::PrintNetwork(this->network_);
}

// static methods

void WiFiManager::PrintMACAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

void WiFiManager::PrintNetwork(WiFiNetwork network) {
  Serial.print("SSID: ");
  Serial.println(network.ssid);

  Serial.print("BSSID: ");
  WiFiManager::PrintMACAddress(network.bssid);

  Serial.print("Signal Strength (RSSI):");
  Serial.print(network.rssi);
  Serial.println(" dBm");

  Serial.print("Encryption Type:");
  Serial.println(network.encryption, HEX);
}