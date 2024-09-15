#pragma once

#include <Wire.h>

struct JustI2C
{
public:
    static void writeRegister(byte addr, byte reg, byte value);

    template <typename T>
    static T readRegister(byte addr, byte reg);
};

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