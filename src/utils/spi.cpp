#pragma once

#include "spi.hpp"

void JustSPI::writeRegister(uint8_t cs, uint8_t reg, uint8_t val)
{
    digitalWrite(cs, LOW);

    SPI.transfer(reg & 0x7F);
    SPI.transfer(val);

    digitalWrite(cs, HIGH);
}
