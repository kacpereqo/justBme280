#pragma once

#include <Wire.h>

struct JustI2C
{
public:
    static void writeRegister(byte addr, byte reg, byte value);

    template <typename T>
    static T readRegister(byte addr, byte reg);
};

#include "i2c.tpp"