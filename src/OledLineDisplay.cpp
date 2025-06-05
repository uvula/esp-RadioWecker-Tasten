
#include <Arduino.h>
#include "OledLineDisplay.h"
#include <algorithm>

OledLineDisplay::OledLineDisplay(uint8_t fixedLineCount, uint8_t scrollLineCount)
    : numFixed(fixedLineCount), numScroll(scrollLineCount)
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

/*
void OledLineDisplay::appendScrollLine(const String& text) {
    if (numScroll == 0) return;

    shiftScrollLines();        // private Methode, nur hier definiert
    scrollLines[numScroll - 1] = text;
    refresh();
}

*/

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
    display.clearDisplay();

    // Zeilenhöhe (z.B. 8 Pixel, abhängig von Font)
    const uint8_t lineHeight = 8;

    // Fixed lines oben zeichnen
    display.setCursor(0, 0);
    for (uint8_t i = 0; i < numFixed; ++i) {
        display.setCursor(0, i * lineHeight);
        display.println(fixedLines[i]);
    }

    // Scroll lines unten zeichnen
    for (uint8_t i = 0; i < numScroll; ++i) {
        display.setCursor(0, (numFixed + i) * lineHeight);
        display.println(scrollLines[i]);
    }

    display.display();
}
