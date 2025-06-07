
#include "OledDisplay.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>

TwoWire& OledDisplay::getDefaultWire() {
    return Wire;
}

OledDisplay::OledDisplay(uint8_t i2cAddr)
    : addr(i2cAddr), display(128, 64, &getDefaultWire()) {}

const char* OledDisplay::name() const {
    return "OLED";
}

uint8_t OledDisplay::address() const {
    return addr;
}

void OledDisplay::onAttach() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, addr)) {
        Serial.println(F("SSD1306 allocation failed"));
    } else {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("OLED Ready");
        display.display();
    }
}

void OledDisplay::showMessage(const String& msg) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(msg);
    display.display();
}

Adafruit_SSD1306& OledDisplay::getDisplay() {
    return display;
}

void OledDisplay::drawField(const OledField& field) {
    Serial.println("Draw");
    display.setCursor(field.getX(), field.getY());
    Serial.println(String(field.getX()));
    Serial.println(String(field.getY()));

    if (field.getMode() == MODE_INVERSE || (field.getMode() == MODE_BLINK && field.getVisible())) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    Serial.println(String(field.getContent()));

    display.print(field.getContent());
    display.display();
  }