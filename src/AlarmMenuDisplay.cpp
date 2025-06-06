#include "AlarmMenuDisplay.h"

AlarmMenuDisplay::AlarmMenuDisplay(OledDisplay& oled)
    : OledLineDisplay(oled, 3, 8 - 3)
{
}

void AlarmMenuDisplay::showClock(const String& timeString) {
    setFixedLine(0, timeString);
}

void AlarmMenuDisplay::setAlarmActive(bool active) {
    alarmActive = active;
    updateFixedLines();
}

void AlarmMenuDisplay::updateAlarmDisplay() {
    updateFixedLines();
}

void AlarmMenuDisplay::updateFixedLines() {
    setFixedLine(1, alarmActive ? "Alarm EIN" : "Alarmeinstellung");
    setFixedLine(2, getAlarmTime());
}

String AlarmMenuDisplay::getAlarmTime() const {
    char buf[9];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", alarmHours, alarmMinutes, alarmSeconds);
    return String(buf);
}

// --- Zeiteinstellung ---

void AlarmMenuDisplay::incrementHours() {
    alarmHours = (alarmHours + 1) % 24;
    updateFixedLines();
}

void AlarmMenuDisplay::decrementHours() {
    alarmHours = (alarmHours + 23) % 24;
    updateFixedLines();
}

void AlarmMenuDisplay::incrementMinutes() {
    alarmMinutes = (alarmMinutes + 1) % 60;
    updateFixedLines();
}

void AlarmMenuDisplay::decrementMinutes() {
    alarmMinutes = (alarmMinutes + 59) % 60;
    updateFixedLines();
}

void AlarmMenuDisplay::incrementSeconds() {
    alarmSeconds = (alarmSeconds + 1) % 60;
    updateFixedLines();
}

void AlarmMenuDisplay::decrementSeconds() {
    alarmSeconds = (alarmSeconds + 59) % 60;
    updateFixedLines();
}
