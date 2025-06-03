#pragma once

#include <Arduino.h>

class WifiInfo {
private:
  String ssid;
  String password;
  String ip; // darf später gesetzt werden

public:
  WifiInfo(const String &ssid, const String &password)
      : ssid(ssid), password(password), ip("") {}

  void setIP(const String &ipAddress) { this->ip = ipAddress; }

  String getSSID() const { return ssid; }

  String getPassword() const { return password; }

  String getIP() const { return ip; }
};
