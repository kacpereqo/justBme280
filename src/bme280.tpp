#pragma once

#include "bme280.hpp"
#include "utils/i2c.hpp"
#include "utils/spi.hpp"

/*!
 * @brief Read a register from an I2C or SPI device, it depends on `this->config.communication` enumeration.
 *
 * @param reg Register to read.
 * @tparam return type of the register, it will read `sizeof(T)` registers ahead.
 * @return Register value.
 */
template <typename T>
T BME280::readRegister(byte reg)
{
    if (this->config.communication == BME280Config::Communication::i2c)
        return JustI2C::readRegister<T>(this->config.addr, reg);
    else
        return JustSPI::readRegister<T>(this->config.cs_pin, reg);
}
