#pragma once

#include "spi.hpp"

/*!
 * @brief read a register from an SPI device, usually function interface provides `size_t len` argument but here `len` depends on `sizeof(T)`.
 *
 * @param cs Chip select pin.
 * @param reg Register to read.
 * @tparam return type of the register, it will read `sizeof(T)` registers ahead.
 * @return Register value.
 */
template <typename T>
inline T JustSPI::readRegister(uint8_t cs, uint8_t reg)
{
    digitalWrite(cs, LOW);

    SPI.transfer((reg & 0x7F) | 0x80);

    byte data[sizeof(T)];

    for (int i = 0; i < sizeof(T); i++)
        data[i] = SPI.transfer(0x00);

    digitalWrite(cs, HIGH);

    return *reinterpret_cast<T *>(data);
}