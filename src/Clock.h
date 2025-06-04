#pragma once

#include <time.h>
#include "NtpClient.h"
#include "OledLineDisplay.h"

class Clock {
public:
    Clock(NtpClient& ntpClient, OledLineDisplay& display);

    void begin();
    void loop();

private:
    NtpClient& ntp;
    OledLineDisplay& oled;

    time_t lastUpdateTime = 0;        // Letzte Anzeigeaktualisierung
    time_t lastSyncTime = 0;          // Letzte NTP-Synchronisierung
    const unsigned long updateInterval = 1000;      // Anzeige alle 1 Sekunde aktualisieren
//    const unsigned long syncInterval = 10 * 60 * 1000; // Alle 10 Minuten NTP sync
    const unsigned long syncInterval = 10 * 60 * 1000; // Alle 10 Minuten NTP sync
    time_t lastNtpTime = 0;         // NTP-Zeit beim letzten Sync
    unsigned long lastMillisSync = 0; // millis() beim letzten Sync

    void updateDisplay();
    void synchronizeTime();
    long getDrift();

};
