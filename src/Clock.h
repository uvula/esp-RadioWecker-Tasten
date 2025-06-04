#pragma once

#include <time.h>
#include "NtpClient.h"
#include "OledLineDisplay.h"
#include "IntervalTrigger.h"

class Clock {
public:
    Clock(NtpClient& ntpClient, OledLineDisplay& display);

    void begin();
    void loop();

private:
    NtpClient& ntp;
    OledLineDisplay& oled;
    IntervalTrigger syncTrigger;
    IntervalTrigger displayTrigger;

    time_t initialNtpTime = 0;
    unsigned long initialMillis = 0;

    void updateDisplay();
    void synchronizeTime();
    long getDrift();

};
