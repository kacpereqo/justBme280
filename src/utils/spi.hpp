#pragma once

#include <SPI.h>
#include <cstdint>

struct JustSPI
{
public:
    static void writeRegister(uint8_t cs, uint8_t reg, uint8_t val);

    template <typename T>
    static T readRegister(uint8_t cs, uint8_t reg);
};

#include "spi.tpp"