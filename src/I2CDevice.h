#pragma once

#include <Arduino.h>

class I2CManager;  // Forward Declaration

class I2CDevice {
protected:
    I2CManager* manager = nullptr;

public:
    virtual const char* name() const = 0;
    virtual uint8_t address() const = 0;

    void attachManager(I2CManager* mgr) {
        this->manager = mgr;
    }

    I2CManager* getManager() const {
        return manager;
    }

    virtual void onAttach() {}
};
