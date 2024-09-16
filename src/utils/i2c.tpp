#pragma once

#include "i2c.hpp"

template <typename T>
T JustI2C::readRegister(byte addr, byte reg)
{
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(addr, sizeof(T));

    byte data[sizeof(T)];

    for (int i = 0; i < sizeof(T); i++)
        if (Wire.available())
            data[i] = Wire.read();
    return *reinterpret_cast<T *>(data);
}