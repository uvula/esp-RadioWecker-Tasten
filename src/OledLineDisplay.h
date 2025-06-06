#pragma once
#include "OledDisplay.h"
#include <Arduino.h>

class OledLineDisplay {
public:
    OledLineDisplay(OledDisplay& oled, uint8_t fixedLineCount, uint8_t scrollLineCount);
    ~OledLineDisplay();

    void setFixedLine(uint8_t index, const String& text);
    void appendScrollLine(const String& text);

    void clearFixed();
    void clearScroll();
    void clearAll();

    void refresh();

protected:
    OledDisplay& oled;
    uint8_t numFixed;
    uint8_t numScroll;

    String* fixedLines;
    String* scrollLines;

    void shiftScrollLines();


};
