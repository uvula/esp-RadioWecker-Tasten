#pragma once

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <WiFiClientSecure.h>
#include <functional>
#include "WifiInfo.h"
#include "OledLineDisplay.h"

class WifiManager {
public:
    WifiManager(OledLineDisplay& oled, const WifiInfo& configInfo);
    void begin(); // Automatisch: Verbinden oder Konfigurationsmodus starten
    void loop();  // WebServer handleClient

    WiFiClientSecure& getSecureClient();
    const String statusToString();

    void setOnConfigStart(std::function<void(const WifiInfo& wifiInfo)> callback);

private:
    OledLineDisplay& oled;
    WifiInfo configMe;
    WifiInfo me;
    WiFiClientSecure client;
    WebServer server{80};
    Preferences preferences;
    bool inConfigMode = false;  // Status merken

    std::function<void(const WifiInfo& wifiInfo)> onConfigStartCallback;
    void showConfigureMe(const WifiInfo &wifiInfo);

    void startConfigPortal();
    bool tryConnectStored();

    void handleRoot();
    void handleSave();
};
