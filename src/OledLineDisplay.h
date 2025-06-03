#pragma once

#include "OledDisplay.h"
#include <array>

class OledLineDisplay : public OledDisplay {
public:
    static constexpr uint8_t MaxLines = 8; // 64px / 8px (bei TextSize 1)

    OledLineDisplay();

    void showLineMessage(uint8_t line, const String& message);
    void appendLine(const String& message);

    void clearLine(uint8_t line);
    void clearAll();

private:
    std::array<String, MaxLines> lines;

    void redraw();
};
