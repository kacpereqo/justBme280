#pragma once

#include <SPI.h>
#include <cstdint>

struct JustSPI
{
public:
    static void writeRegister(uint8_t cs, uint8_t reg, uint8_t val)
    {
        digitalWrite(cs, LOW);

        SPI.transfer(reg & 0x7F);
        SPI.transfer(val);

        digitalWrite(cs, HIGH);
    }

    template <typename T>
    static T readRegister(uint8_t cs, uint8_t reg)
    {
        digitalWrite(cs, LOW);

        SPI.transfer((reg & 0x7F) | 0x80);

        byte data[sizeof(T)];

        for (int i = 0; i < sizeof(T); i++)
            data[i] = SPI.transfer(0x00);

        digitalWrite(cs, HIGH);

        return *reinterpret_cast<T *>(data);
    }
};