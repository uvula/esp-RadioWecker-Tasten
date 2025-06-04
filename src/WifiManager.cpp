#include "WifiManager.h"
#include <functional>

WifiManager::WifiManager(OledLineDisplay &oled, const WifiInfo &configInfo) : oled(oled), configMe(configInfo), me(WifiInfo("", ""))
{
    client.setInsecure(); // Für HTTPS ohne Zertifikatsprüfung
    // setOnConfigStart(std::bind(&WifiManager::showConfigureMe, this, configMe));
    setOnConfigStart([this](const WifiInfo &)
                     { showConfigureMe(configMe); });
}

void WifiManager::begin()
{
    oled.setFixedLine(0, String("Starting WLAN"));
    WiFi.setHostname("esp32-firstUnit");

    if (!tryConnectStored())
    {
        startConfigPortal();
    }
}

bool WifiManager::tryConnectStored()
{
    preferences.begin("wifi", true); // Read-only
    String ssid = preferences.getString("ssid", "");
    String pass = preferences.getString("pass", "");
    preferences.end();

    if (ssid.isEmpty())
        return false;

    me = WifiInfo(ssid, pass);
    Serial.print("Versuche Verbindung zu: ");
    Serial.println(ssid);
    oled.appendScrollLine("Versuche Verbindung zu: ");
    oled.appendScrollLine(ssid);
    const int maxAttempts = 3;
    for (int attempt = 1; attempt <= maxAttempts; ++attempt)
    {
        Serial.printf("Verbindungsversuch %d von %d...\n", attempt, maxAttempts);
        oled.appendScrollLine(String("Verbindungsversuch ") + attempt + " von " + maxAttempts);
        WiFi.begin(ssid.c_str(), pass.c_str());

        unsigned long start = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - start < 10000)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println();

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("WLAN verbunden: " + WiFi.localIP().toString());
            oled.appendScrollLine(String("WLAN verbunden: ") + WiFi.localIP().toString());
            me.setIP(WiFi.localIP().toString());
            return true;
        }
        else
        {
            Serial.println("Verbindung fehlgeschlagen.");
            oled.appendScrollLine("Verbindung fehlgeschlagen.");
            WiFi.disconnect(true);
            delay(1000); // Kleine Pause vor erneutem Versuch
        }
    }

    Serial.println("Alle Verbindungsversuche fehlgeschlagen.");
    return false;
}

void WifiManager::startConfigPortal()
{
    Serial.println("Kein WLAN, starte Konfigurationsmodus...");
    WiFi.softAP(configMe.getSSID(), configMe.getPassword());
    IPAddress IP = WiFi.softAPIP();
    configMe.setIP(IP.toString());
    Serial.print("AP SSID: ");
    Serial.print(configMe.getSSID());
    Serial.print(" IP-Adresse: ");
    Serial.println(configMe.getIP());
    inConfigMode = true;
    if (onConfigStartCallback)
    {
        onConfigStartCallback(configMe);
    }

    server.on("/", std::bind(&WifiManager::handleRoot, this));
    server.on("/save", HTTP_POST, std::bind(&WifiManager::handleSave, this));
    server.begin();
}

void WifiManager::handleRoot()
{
    String html = "<form action='/save' method='POST'>"
                  "SSID: <input type='text' name='ssid'><br>"
                  "Passwort: <input type='password' name='password'><br>"
                  "<input type='submit' value='Speichern'>"
                  "</form>";
    server.send(200, "text/html", html);
}

void WifiManager::handleSave()
{
    if (server.hasArg("ssid") && server.hasArg("password"))
    {
        String ssid = server.arg("ssid");
        String pass = server.arg("password");

        preferences.begin("wifi", false);
        preferences.putString("ssid", ssid);
        preferences.putString("pass", pass);
        preferences.end();

        server.send(200, "text/html", "<h2>Gespeichert. Neustart...</h2>");
        delay(3000);
        ESP.restart();
    }
    else
    {
        server.send(400, "text/plain", "Fehlende Parameter");
    }
}

WiFiClientSecure &WifiManager::getSecureClient()
{
    return client;
}

const String WifiManager::statusToString()
{
    switch (WiFi.status())
    {
    case WL_IDLE_STATUS:
        return "Idle";
    case WL_NO_SSID_AVAIL:
        return "No SSID";
    case WL_SCAN_COMPLETED:
        return "Scan Completed";
    case WL_CONNECTED:
        return "Connected";
    case WL_CONNECT_FAILED:
        return "Failed";
    case WL_CONNECTION_LOST:
        return "Lost";
    case WL_DISCONNECTED:
        return "Disconnected";
    default:
        return "Unknown";
    }
}

void WifiManager::setOnConfigStart(std::function<void(const WifiInfo &wifiInfo)> callback)
{
    this->onConfigStartCallback = callback;
}

void WifiManager::loop()
{
    while (inConfigMode)
    {
        server.handleClient();
    }
    oled.appendScrollLine(String("WLAN: ") + statusToString());
}

void WifiManager::showConfigureMe(const WifiInfo &wifiInfo)
{
    // Hier z. B. OLED-Text: "WLAN-Konfiguration..."
    Serial.println("OLED zeigt Konfigurationsmodus");
    oled.clearAll();
    oled.setFixedLine(1, "Configure me");
    oled.appendScrollLine("IP: " + configMe.getIP());
    oled.appendScrollLine("SSID: " + configMe.getSSID());
    oled.appendScrollLine("PWD: " + configMe.getPassword());
    oled.appendScrollLine("Connect to Wifi and");
    oled.appendScrollLine("fill mask");
}
