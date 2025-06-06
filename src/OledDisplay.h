#pragma once

#include "I2CDevice.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class OledDisplay final : public I2CDevice {
private:
    static TwoWire& getDefaultWire();
    uint8_t addr;

protected:
    Adafruit_SSD1306 display;

public:
    OledDisplay(uint8_t i2cAddr = 0x3C);
    void onAttach() override;
    void showMessage(const String& msg);
    const char* name() const override;
    uint8_t address() const override;
    Adafruit_SSD1306& getDisplay();
};
