#include "Clock.h"
#include <Arduino.h>

Clock::Clock(NtpClient &ntpClient, OledLineDisplay &display)
    : ntp(ntpClient), oled(display) {}

void Clock::begin() {
  ntp.begin();
  if (ntp.waitForTime()) {
    lastSyncTime = millis();
    lastUpdateTime = 0; // sofort Anzeige updaten
  }
  synchronizeTime();
}

void Clock::loop() {
  static int x = 0;
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
    struct tm *timeinfo = localtime(&currentTime);
    char buf[20];
    strftime(buf, sizeof(buf), "%H:%M:%S", timeinfo);
    oled.setFixedLine(0, String(buf));
    long driftMs = getDrift();
    oled.appendScrollLine("Drift(ms): " + String(driftMs));
  } else {
    oled.setFixedLine(0, "Zeit nicht valid");
  }
  oled.refresh();
}

void Clock::synchronizeTime() {
  oled.appendScrollLine("Sync mit NTP...");
  ntp.begin(); // configTime() neu setzen
  if (ntp.waitForTime(10000)) {
    oled.appendScrollLine("NTP sync OK");
    lastNtpTime = ntp.now();
    lastMillisSync = millis();
  } else {
    oled.appendScrollLine("NTP sync fehlgeschl.");
  }
}

long Clock::getDrift() {
  if (!ntp.isTimeValid() || lastNtpTime == 0 || lastMillisSync == 0) {
    // Noch keine gültige Synchronisierung
    return 0;
  }
  time_t currentNtp = ntp.now();
  unsigned long currentMillis = millis();

  // Verstrichene Zeit nach NTP (Sekunden)
  time_t ntpElapsed = currentNtp - lastNtpTime;

  // Verstrichene lokale Zeit (Millisekunden)
  unsigned long localElapsedMs = currentMillis - lastMillisSync;

  // Lokale Zeit in Sekunden
  float localElapsedSec = localElapsedMs / 1000.0f;

  // Drift = lokale Zeit - NTP-Zeit
  float driftSeconds = localElapsedSec - ntpElapsed;

  return static_cast<long>(driftSeconds * 1000); // in ms (optional)
}
