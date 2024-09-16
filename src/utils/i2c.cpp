#include "i2c.hpp"

/*!
 * @brief Write a register to an I2C device.
 *
 * @param addr I2C address.
 * @param reg Register to write.
 * @param value Value to write.
 */
void JustI2C::writeRegister(byte addr, byte reg, byte value)
{
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}
