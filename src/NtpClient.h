#pragma once

#include <time.h>
#include "OledLineDisplay.h"

class NtpClient {
public:
    NtpClient(OledLineDisplay& display,
              const char* ntpServer = "pool.ntp.org",
              int gmtOffsetSeconds = 3600,
              int daylightOffsetSeconds = 3600);

    void begin();                                  // Startet Zeitabfrage (einmalig)
    bool waitForTime(int timeoutMillis = 10000);  // Wartet auf gültige Zeit
    void update();                                 // Optional: prüfen/refresh (leer oder minimal)
    time_t now();                                  // Aktuelle Zeit (time_t)
    String timeString();                           // Formatierte Zeit
    void showTime();                               // OLED-Ausgabe
    bool isTimeValid() const;                      // Zeit wurde synchronisiert

private:
    OledLineDisplay& oled;
    const char* server;
    int gmtOffset;
    int daylightOffset;
    bool timeValid = false;                         // Status speichern
};
