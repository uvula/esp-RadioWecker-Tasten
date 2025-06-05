#pragma once

#include <Arduino.h>
#include <functional>

class IntervalTrigger {
  public:
    IntervalTrigger(unsigned long intervalMillis)
      : interval(intervalMillis), lastTrigger(0 - intervalMillis) {}

    // Führt die Aktion aus, wenn das Intervall abgelaufen ist
    void check(std::function<void()> action) {
        unsigned long now = millis();
        if (now - lastTrigger >= interval) {
            lastTrigger = now;
            action();  // führe Lambda aus
        }
    }

    void reset() {
        lastTrigger = millis();
    }

    void setInterval(unsigned long newInterval) {
        interval = newInterval;
    }

  private:
    unsigned long interval;
    unsigned long lastTrigger;
};
