#include "NtpClient.h"

NtpClient::NtpClient(OledLineDisplay& display,
                     const char* ntpServer,
                     int gmtOffsetSeconds,
                     int daylightOffsetSeconds)
    : oled(display), server(ntpServer),
      gmtOffset(gmtOffsetSeconds),
      daylightOffset(daylightOffsetSeconds) {}

void NtpClient::begin() {
    oled.appendScrollLine("NTP: Verbinde...");
    configTime(gmtOffset, daylightOffset, server);
}

bool NtpClient::waitForTime(int timeoutMillis) {
    time_t nowTime;
    int waited = 0;

    while ((nowTime = time(nullptr)) < 8 * 3600 * 2) {  // Arbiträrer Check, Zeit muss > 16h sein
        delay(200);
        waited += 200;
        if (waited >= timeoutMillis) {
            oled.appendScrollLine("NTP: Timeout");
            timeValid = false;
            return false;
        }
    }

    oled.appendScrollLine("NTP: Zeit erhalten");
    timeValid = true;
    return true;
}

void NtpClient::update() {
    // Bei dieser Implementierung reicht es, Zeit über time() abzufragen.
    // Hier könnte man später Erweiterungen hinzufügen.
}

time_t NtpClient::now() {
    return time(nullptr);
}

bool NtpClient::isTimeValid() const {
    return timeValid;
}

String NtpClient::timeString() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return "Zeitfehler";

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(buffer);
}

void NtpClient::showTime() {
    oled.appendScrollLine(timeString());
}
