#include "Clock.h"
#include <Arduino.h>

Clock::Clock(NtpClient& ntpClient, OledLineDisplay& display)
    : ntp(ntpClient), oled(display) {}

void Clock::begin() {
    ntp.begin();
    if (ntp.waitForTime()) {
        lastSyncTime = millis();
        lastUpdateTime = 0; // sofort Anzeige updaten
    }
}

void Clock::loop() {
    unsigned long nowMillis = millis();

    // Periodisch NTP neu synchronisieren
    if (nowMillis - lastSyncTime >= syncInterval) {
        synchronizeTime();
        lastSyncTime = nowMillis;
    }

    // Anzeige alle 1 Sekunde updaten
    if (nowMillis - lastUpdateTime >= updateInterval) {
        updateDisplay();
        lastUpdateTime = nowMillis;
    }

    // Optionale regelmäßige NTP-Update-Calls (z.B. keep alive)
    ntp.update();
}

void Clock::updateDisplay() {
    if (ntp.isTimeValid()) {
        time_t currentTime = ntp.now();
        struct tm* timeinfo = localtime(&currentTime);
        char buf[20];
        strftime(buf, sizeof(buf), "%H:%M:%S", timeinfo);
        oled.setFixedLine(0, String(buf));
    } else {
        oled.setFixedLine(0, "Zeit nicht valid");
    }
    oled.refresh();
}

void Clock::synchronizeTime() {
    oled.appendScrollLine("Sync mit NTP...");
    ntp.begin();  // configTime() neu setzen
    if (ntp.waitForTime(10000)) {
        oled.appendScrollLine("NTP sync OK");
    } else {
        oled.appendScrollLine("NTP sync fehlgeschl.");
    }
}
