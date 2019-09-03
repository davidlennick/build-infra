#ifndef WIFIMANAGER_H_
#define WIFIMANAGER_H_

#include "WiFiNINA.h"
#include "WiFiNetwork.h"
#include "arduino_secrets.h"

class WiFiManager {
 public:
  WiFiManager(WiFiClient* client);

  WiFiNetwork current_network();
  WiFiClass* wifi();

  void Init();
  bool ConnectWiFi(byte attempt_limit=5);
  bool IsWiFiConnected();
  void Disconnect();
  void UpdateCurrentNetwork();
  void PrintWiFiClientData();
  void PrintCurrentNetwork();

  // void CreateWebClient();
  // void GET(client, String[] content);
  // void PUT()
  // void ReadResponse();

  static void PrintMACAddress(byte mac[]);
  static void PrintNetwork(WiFiNetwork network);

 private:
  WiFiClass* wifi_;
  WiFiClient* client_;
  WiFiNetwork network_;
};

#endif