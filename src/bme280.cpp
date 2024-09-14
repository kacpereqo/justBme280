#include "bme280.hpp"
#include "utils/i2c.hpp"

bool BME280::begin()
{
    Wire.begin();

    byte chipId = this->getChipId();

    constexpr byte BME280_CHIP_ID = 0x60;
    constexpr byte BMP_280_CHIP_ID = 0x58;

    if (chipId != BME280_CHIP_ID && chipId != BMP_280_CHIP_ID)
        return false;

    this->writeConfig();
    this->readCalibraionData();

    return true;
}

void BME280::update()
{
    rawData readData = this->readRegister<rawData>(BME280_PRESSURE_MSB_REG);

    this->adc_T = (readData.temperature[0] << 12) | (readData.temperature[1] << 4) | (readData.temperature[2] >> 4);
    this->adc_P = (readData.pressure[0] << 12) | (readData.pressure[1] << 4) | (readData.pressure[2] >> 4);
    this->adc_H = (readData.humidity[0] << 8) | readData.humidity[1];
}

void BME280::reset()
{
    this->writeRegister(BME280_RST_REG, BME280_RST_REG);
}

template <typename T>
T BME280::readRegister(byte reg)
{
    return I2C::readRegister<T>(this->config.addr, reg);
}

void BME280::writeRegister(byte reg, byte val)
{
    I2C::writeRegister(this->config.addr, reg, val);
}

void BME280::setConfig(Config config)
{
    this->config = config;
    this->writeConfig();
}

int32_t BME280::getTemperatureRaw(int32_t adc_T)
{

    int32_t var1, var2, T;

    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;

    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;

    return T;
}

int32_t BME280::getPressureRaw(int32_t adc_P)
{

    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
    if (var1 == 0)
    {
        return 0;
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
    return (int64_t)p;
}

uint32_t BME280::getHumidityRaw(int32_t adc_H)
{
    int32_t var1;

    var1 = (t_fine - ((int32_t)76800));
    var1 = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * var1)) +
              ((int32_t)16384)) >>
             15) *
            (((((((var1 * ((int32_t)dig_H6)) >> 10) * (((var1 *
                                                         ((int32_t)dig_H3)) >>
                                                        11) +
                                                       ((int32_t)32768))) >>
                10) +
               ((int32_t)2097152)) *
                  ((int32_t)dig_H2) +
              8192) >>
             14));
    var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
    var1 = (var1 < 0 ? 0 : var1);
    var1 = (var1 > 419430400 ? 419430400 : var1);
    return ((uint32_t)(var1 >> 12));
}

float BME280::getTemperature()
{
    constexpr float divide_factor = 100.0f;
    return getTemperatureRaw(this->adc_T) / divide_factor;
}

float BME280::getPressure()
{
    constexpr float divide_factor = 256.0f * 100.0f;
    return getPressureRaw(this->adc_P) / divide_factor;
}

float BME280::getHumidity()
{
    constexpr float divide_factor = 1024.0f;
    return getHumidityRaw(this->adc_H) / divide_factor;
}

float BME280::getMesurementTime()
{
    float time = 1.25f;

    uint8_t osrs_t = static_cast<uint8_t>(this->config.temperature_oversampling);
    uint8_t osrs_p = static_cast<uint8_t>(this->config.pressure_oversampling);
    uint8_t osrs_h = static_cast<uint8_t>(this->config.humidity_oversampling);

    float t_conv = osrs_t != 0 ? 0.0f : 2.3f * osrs_t;
    float p_conv = osrs_p != 0 ? 0.0f : 2.3f * osrs_p + 0.575f;
    float h_conv = osrs_h != 0 ? 0.0f : 2.3f * osrs_h + 0.575f;

    return time + t_conv + p_conv + h_conv;
}

void BME280::readCalibraionData()
{

    this->dig_T1 = this->readRegister<uint16_t>(BME280_DIG_T1_LSB_REG);
    this->dig_T2 = this->readRegister<int16_t>(BME280_DIG_T2_LSB_REG);
    this->dig_T3 = this->readRegister<int16_t>(BME280_DIG_T3_LSB_REG);

    this->dig_P1 = this->readRegister<uint16_t>(BME280_DIG_P1_LSB_REG);
    this->dig_P2 = this->readRegister<int16_t>(BME280_DIG_P2_LSB_REG);
    this->dig_P3 = this->readRegister<int16_t>(BME280_DIG_P3_LSB_REG);
    this->dig_P4 = this->readRegister<int16_t>(BME280_DIG_P4_LSB_REG);
    this->dig_P5 = this->readRegister<int16_t>(BME280_DIG_P5_LSB_REG);
    this->dig_P6 = this->readRegister<int16_t>(BME280_DIG_P6_LSB_REG);
    this->dig_P7 = this->readRegister<int16_t>(BME280_DIG_P7_LSB_REG);
    this->dig_P8 = this->readRegister<int16_t>(BME280_DIG_P8_LSB_REG);
    this->dig_P9 = this->readRegister<int16_t>(BME280_DIG_P9_LSB_REG);

    this->dig_H1 = this->readRegister<uint8_t>(BME280_DIG_H1_REG);
    this->dig_H2 = this->readRegister<int16_t>(BME280_DIG_H2_LSB_REG);
    this->dig_H3 = this->readRegister<uint8_t>(BME280_DIG_H3_REG);
    this->dig_H4 = (this->readRegister<int8_t>(BME280_DIG_H4_MSB_REG) << 4) | (this->readRegister<int8_t>(BME280_DIG_H4_LSB_REG) & 0x0F);
    this->dig_H5 = (this->readRegister<int8_t>(BME280_DIG_H5_MSB_REG) << 4) | (this->readRegister<int8_t>(BME280_DIG_H5_MSB_REG) >> 4);
    this->dig_H6 = this->readRegister<int8_t>(BME280_DIG_H6_REG);
}

void BME280::writeConfig()
{
    byte ctrl_meas = static_cast<byte>(this->config.temperature_oversampling) << 5 |
                     static_cast<byte>(this->config.pressure_oversampling) << 2 |
                     static_cast<byte>(this->config.mode);

    byte ctrl_hum = static_cast<byte>(this->config.humidity_oversampling);

    byte config = static_cast<byte>(this->config.standby) << 5 |
                  static_cast<byte>(this->config.filter) << 2 |
                  static_cast<byte>(this->config.spi_3);

    this->writeRegister(BME280_CONFIG_REG, config);
    this->writeRegister(BME280_CTRL_HUMIDITY_REG, ctrl_hum);
    this->writeRegister(BME280_CTRL_MEAS_REG, ctrl_meas);
}

byte BME280::getChipId()
{
    return this->readRegister<byte>(BME280_CHIP_ID_REG);
}
