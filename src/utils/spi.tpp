#pragma once

#include "spi.hpp"

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