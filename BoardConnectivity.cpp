
#include "BoardConnectivity.h"

BoardConnectivityClass BoardConnectivity;

void BoardConnectivityClass::begin(
    HardwareSerial &port, uint32_t baud) {
  _serial = &port;
  _serial->begin(baud);
}

String BoardConnectivityClass::readLine(uint32_t timeoutMs) {
  if (!_serial) return "";

  String response;
  uint32_t start = millis();

  while (millis() - start < timeoutMs) {
    while (_serial->available()) {
      char c = (char)_serial->read();

      if (c == '\n') {
        response.trim();
        return response;
      }

      if (c != '\r') {
        response += c;
      }
    }
    delay(1);
  }

  response.trim();
  return response;
}

String BoardConnectivityClass::sendCommand(
    const String &cmd, uint32_t timeoutMs) {
  if (!_serial) return "ERROR|UART_NOT_INITIALIZED";

  // Remove any stale input before sending a new command.
  while (_serial->available()) {
    _serial->read();
  }

  _serial->println(cmd);
  return readLine(timeoutMs);
}

String BoardConnectivityClass::command(
    const String &cmd, uint32_t timeoutMs) {
  return sendCommand(cmd, timeoutMs);
}

bool BoardConnectivityClass::ping() {
  return sendCommand("PING", 2000).startsWith("PONG");
}

String BoardConnectivityClass::wifiStatus() {
  return sendCommand("WIFI_STATUS", 3000);
}

String BoardConnectivityClass::wifiConnect(
    const String &ssid, const String &password) {
  if (ssid.indexOf('|') >= 0 || password.indexOf('|') >= 0) {
    return "ERROR|SSID_OR_PASSWORD_CONTAINS_PIPE";
  }

  return sendCommand(
      "WIFI_CONNECT|" + ssid + "|" + password, 20000);
}

String BoardConnectivityClass::wifiDisconnect() {
  return sendCommand("WIFI_DISCONNECT", 5000);
}

String BoardConnectivityClass::wifiScan() {
  return sendCommand("WIFI_SCAN", 20000);
}

String BoardConnectivityClass::bleStart() {
  return sendCommand("BLE_START", 10000);
}

String BoardConnectivityClass::bleStatus() {
  return sendCommand("BLE_STATUS", 3000);
}

String BoardConnectivityClass::bleScan(uint8_t seconds) {
  if (seconds < 1) seconds = 1;
  if (seconds > 20) seconds = 20;

  return sendCommand("BLE_SCAN|" + String(seconds),
                     (uint32_t)seconds * 1000 + 5000);
}

String BoardConnectivityClass::bleRead() {
  return sendCommand("BLE_READ", 3000);
}

String BoardConnectivityClass::bleWrite(
    const String &message) {
  if (message.indexOf('|') >= 0) {
    return "ERROR|MESSAGE_CONTAINS_PIPE";
  }

  return sendCommand("BLE_WRITE|" + message, 5000);
}
