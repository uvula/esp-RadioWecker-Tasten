#include "OledLineDisplay.h"

OledLineDisplay::OledLineDisplay() {
    lines.fill(""); // alle Zeilen leer
}

void OledLineDisplay::showLineMessage(uint8_t line, const String& message) {
    if (line >= MaxLines) return;
    lines[line] = message;
    redraw();
}

void OledLineDisplay::clearLine(uint8_t line) {
    if (line >= MaxLines) return;
    lines[line] = "";
    redraw();
}

void OledLineDisplay::clearAll() {
    lines.fill("");
    redraw();
}

void OledLineDisplay::redraw() {
    display.clearDisplay();
    for (uint8_t i = 0; i < MaxLines; ++i) {
        if (!lines[i].isEmpty()) {
            display.setCursor(0, i * 8);
            display.print(lines[i]);
        }
    }
    display.display();
}

void OledLineDisplay::appendLine(const String& message) {
    // Alle Zeilen eine Zeile nach oben schieben
    for (uint8_t i = 0; i < MaxLines - 1; ++i) {
        lines[i] = lines[i + 1];
    }
    // Neue Zeile unten einfügen
    lines[MaxLines - 1] = message;
    redraw();
}
