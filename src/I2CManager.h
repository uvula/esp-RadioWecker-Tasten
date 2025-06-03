#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <vector>
#include "I2CDevice.h"

class I2CManager {
private:
    TwoWire& wire;
    uint8_t sda, scl;
    std::vector<I2CDevice*> devices;

public:
    I2CManager(TwoWire& w, uint8_t sdaPin, uint8_t sclPin)
        : wire(w), sda(sdaPin), scl(sclPin) {}

    void begin() {
        wire.begin(sda, scl);
    }

    void registerDevice(I2CDevice* device) {
        device->attachManager(this);
        devices.push_back(device);
        device->onAttach();
    }

    const std::vector<I2CDevice*>& getDevices() const {
        return devices;
    }

    TwoWire& getBus() {
        return wire;
    }

    void printDevices(Stream& out = Serial) const {
        out.println("I2C Devices:");
        for (auto* device : devices) {
            out.print(" - ");
            out.print(device->name());
            out.print(" @ 0x");
            out.println(device->address(), HEX);
        }
    }   
};
