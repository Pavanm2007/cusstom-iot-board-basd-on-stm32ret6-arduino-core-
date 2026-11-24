
#ifndef BOARD_CONNECTIVITY_H
#define BOARD_CONNECTIVITY_H

#include <Arduino.h>

class BoardConnectivityClass {
public:
  void begin(HardwareSerial &port, uint32_t baud = 115200);

  bool ping();
  String command(const String &cmd, uint32_t timeoutMs = 3000);

  String wifiStatus();
  String wifiConnect(const String &ssid, const String &password);
  String wifiDisconnect();
  String wifiScan();

  String bleStart();
  String bleStatus();
  String bleScan(uint8_t seconds = 5);
  String bleRead();
  String bleWrite(const String &message);

private:
  HardwareSerial *_serial = nullptr;
  String readLine(uint32_t timeoutMs);
  String sendCommand(const String &cmd, uint32_t timeoutMs);
};

extern BoardConnectivityClass BoardConnectivity;

#endif
