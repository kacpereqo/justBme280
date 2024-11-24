#pragma once

#include "i2c.hpp"

/*!
 * @brief read a register from an I2C device, usually function interface provides `size_t len` argument but here `len` depends on `sizeof(T)`.
 *
 * @param addr I2C address.
 * @param reg Register to read.
 * @tparam return type of the register, it will read `sizeof(T)` registers ahead.
 * @return Register value.
 */
template <typename T>
T JustI2C::readRegister(byte addr, byte reg)
{
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(addr, sizeof(T));

    T result;

    for (int i = 0; i < sizeof(T); i++)
        if (Wire.available())
            result |= static_cast<T>(Wire.read() << (8 * i));
    return result;
}
