#pragma once
#include "OledLineDisplay.h"

class AlarmMenuDisplay : public OledLineDisplay {
public:
    AlarmMenuDisplay(OledDisplay& oled);

    void showClock(const String& timeString); // Zeile 0
    void setAlarmActive(bool active);         // Statusanzeige in Zeile 1
    void updateAlarmDisplay();                // Zeit anzeigen in Zeile 2

    void incrementHours();
    void decrementHours();
    void incrementMinutes();
    void decrementMinutes();
    void incrementSeconds();
    void decrementSeconds();

    String getAlarmTime() const;

private:
    int alarmHours = 6;
    int alarmMinutes = 0;
    int alarmSeconds = 0;
    bool alarmActive = false;

    void updateFixedLines();
};
