
#include <Arduino.h>
#include "OledLineDisplay.h"
#include <algorithm>

OledLineDisplay::OledLineDisplay(OledDisplay& oled, uint8_t fixedLineCount, uint8_t scrollLineCount)
    : oled(oled), numFixed(fixedLineCount), numScroll(scrollLineCount)
{
    fixedLines = new String[numFixed];
    scrollLines = new String[numScroll];
    clearFixed();
    clearScroll();
}

OledLineDisplay::~OledLineDisplay() {
    delete[] fixedLines;
    delete[] scrollLines;
}

void OledLineDisplay::setFixedLine(uint8_t index, const String& text) {
    if (index < numFixed) {
        fixedLines[index] = text;
        refresh();
    }
}

// **Private Methode nur in .cpp definieren, nicht im Header**
void OledLineDisplay::shiftScrollLines() {
    for (uint8_t i = 0; i < numScroll - 1; ++i) {
        scrollLines[i] = scrollLines[i + 1];
    }
    scrollLines[numScroll - 1] = "";
}

void OledLineDisplay::appendScrollLine(const String& text) {
    if (numScroll == 0) return;

    const int maxLineLength = 21;  // oder passend zu deinem OLED-Display

    int start = 0;
    while (start < text.length()) {
        int end = std::min(start + maxLineLength, static_cast<int>(text.length()));
        String line = text.substring(start, end);

        shiftScrollLines();
        scrollLines[numScroll - 1] = line;

        start += maxLineLength;
    }

    refresh();
}


void OledLineDisplay::clearFixed() {
    for (uint8_t i = 0; i < numFixed; ++i) {
        fixedLines[i] = "";
    }
}

void OledLineDisplay::clearScroll() {
    for (uint8_t i = 0; i < numScroll; ++i) {
        scrollLines[i] = "";
    }
}

void OledLineDisplay::clearAll() {
    clearFixed();
    clearScroll();
}

void OledLineDisplay::refresh() {
    oled.getDisplay().clearDisplay();

    // Zeilenhöhe (z.B. 8 Pixel, abhängig von Font)
    const uint8_t lineHeight = 8;

    // Fixed lines oben zeichnen
    oled.getDisplay().setCursor(0, 0);
    for (uint8_t i = 0; i < numFixed; ++i) {
        oled.getDisplay().setCursor(0, i * lineHeight);
        //oled.getDisplay().setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        oled.getDisplay().println(fixedLines[i]);
    }

    // Scroll lines unten zeichnen
    for (uint8_t i = 0; i < numScroll; ++i) {
        oled.getDisplay().setCursor(0, (numFixed + i) * lineHeight);
        oled.getDisplay().println(scrollLines[i]);
    }

    oled.getDisplay().display();
}
