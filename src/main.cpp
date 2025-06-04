
#include <Arduino.h>
#include "Atm_led.hpp"
#include "I2CManager.h"
#include "OledLineDisplay.h"
#include "WifiManager.h"
#include "NtpClient.h"
#include "Clock.h"

const int PIN_LED_BUILTIN = 2;
const int PIN_SDA = 21;
const int PIN_SCL = 22;

Atm_led led;
I2CManager i2cManager(Wire, PIN_SDA, PIN_SCL); // SDA, SCL
OledLineDisplay oled(2, 6);

WifiInfo configInfo("ESP-Setup", "configure me");
WifiManager wifi(oled, configInfo);
NtpClient ntp(oled);
Clock clockx(ntp, oled);

void setup()
{
    Serial.begin(115200);
    Serial.println("=== Setup ==================================");
    i2cManager.begin();
    i2cManager.registerDevice(&oled);
    Serial.println("Devices:");
    i2cManager.printDevices();
    oled.clearFixed();
    oled.setFixedLine(1, "Setup");
    led.begin(PIN_LED_BUILTIN);
    led.blink(150, 150, 3).start();

    wifi.begin();
    wifi.loop();

    ntp.begin();
    if (ntp.waitForTime()) {
        ntp.showTime();
    } else {
        oled.appendScrollLine("NTP fehlgeschlagen");
    }
    oled.appendScrollLine("Staring clock");
    delay(1000);
    oled.clearFixed();
    clockx.begin();
}

void loop()
{
    clockx.loop(); // regelmäßig aufrufen
    automaton.run();
}
