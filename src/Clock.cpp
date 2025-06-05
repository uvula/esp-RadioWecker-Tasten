#include "Clock.h"
#include <Arduino.h>

Clock::Clock(NtpClient &ntpClient, OledLineDisplay &display)
    : ntp(ntpClient), oled(display), syncTrigger(60UL * 60 * 1000),
      displayTrigger(1000UL) {}

void Clock::begin() {
  ntp.begin();
  if (ntp.waitForTime()) {
    initialNtpTime = ntp.now();
    initialMillis = millis();
  }
}

void Clock::loop() {

  syncTrigger.check([this]() { synchronizeTime(); });

  displayTrigger.check([this]() { updateDisplay(); });

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
  } else {
    oled.setFixedLine(0, "Zeit nicht valid");
  }
  oled.refresh();
}

void Clock::synchronizeTime() {
  oled.appendScrollLine("Sync mit NTP...");
  ntp.begin(); // configTime() neu setzen
  if (ntp.waitForTime(10000)) {
    oled.appendScrollLine(String("NTP sync OK ") + ntp.timeString());
    long driftMs = getDrift();
    oled.appendScrollLine("Drift(ms): " + String(driftMs));
  } else {
    oled.appendScrollLine("NTP sync fehlgeschl.");
  }
}

long Clock::getDrift() {
  time_t currentNtp = ntp.now();
  unsigned long nowMillis = millis();
  long deltaSeconds = currentNtp - initialNtpTime;
  long expectedMillis = initialMillis + deltaSeconds * 1000L;
  return nowMillis - expectedMillis;
}
