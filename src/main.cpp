
#include <Arduino.h>
#include "Atm_led.hpp"
#include "I2CManager.h"
#include "OledLineDisplay.h"
#include "WifiManager.h"
#include "NtpClient.h"
#include "Clock.h"
#include "AlarmMenuDisplay.h"

const int PIN_LED_BUILTIN = 2;
const int PIN_SDA = 21;
const int PIN_SCL = 22;

Atm_led led;
I2CManager i2cManager(Wire, PIN_SDA, PIN_SCL); // SDA, SCL
OledDisplay physicalDisplay;
OledLineDisplay stdDisplay(physicalDisplay, 2, 6);
AlarmMenuDisplay alarmDisplay(physicalDisplay);

WifiInfo configInfo("ESP-Setup", "configure me");
WifiManager wifi(stdDisplay, configInfo);
NtpClient ntp(stdDisplay);
Clock clockx(ntp, stdDisplay);

void setup()
{
    Serial.begin(115200);
    Serial.println("=== Setup ==================================");
    i2cManager.begin();
    i2cManager.registerDevice(&physicalDisplay);
    Serial.println("Devices:");
    i2cManager.printDevices();

    stdDisplay.clearFixed();
    stdDisplay.setFixedLine(1, "Setup");
    led.begin(PIN_LED_BUILTIN);
    led.blink(150, 150, 3).start();
    wifi.begin();
    wifi.loop();

    ntp.begin();
    if (ntp.waitForTime()) {
        ntp.showTime();
    } else {
        stdDisplay.appendScrollLine("NTP fehlgeschlagen");
    }
    stdDisplay.appendScrollLine("Staring clock");
    delay(1000);
    stdDisplay.clearFixed();
    clockx.begin();

    alarmDisplay.showClock("12:34:56");
    alarmDisplay.updateAlarmDisplay();

}

void loop()
{
//    clockx.loop(); // regelmäßig aufrufen
    automaton.run();
}
