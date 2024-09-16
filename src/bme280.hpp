#pragma once

#include "cstdint"
#include "Wire.h"
#include "utils/i2c.hpp"
#include "registers.hpp"
#include "bme280config.hpp"

/*!
 * @brief BME280 sensor class, provides methods to read temperature, pressure and humidity. to initialize the sensor, call begin() method.
 */
struct BME280
{
public:
    bool begin();

    void setConfig(BME280Config::Config config);
    void update();
    void reset();

    // 4.2.3 Compensation formulas

    float getTemperature(); // in Celsius
    float getPressure();    // in hPa
    float getHumidity();    // in %

    float getMesurementTime();

private:
    template <typename T>
    T readRegister(byte reg);
    void writeRegister(byte reg, byte val);

    int32_t getTemperatureRaw(int32_t adc_T);
    int32_t getPressureRaw(int32_t adc_P);
    uint32_t getHumidityRaw(int32_t adc_H);

    struct rawData
    {
        byte pressure[3];
        byte temperature[3];
        byte humidity[2];
    };

    BME280Config::Config config;

    // analog-digital converter raw data
    int32_t adc_T;
    int32_t adc_P;
    int32_t adc_H;

    // Table 16: Compensation parameter storage, naming and data type
    // Calibration data
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;

    // Pressure calibration data
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;

    // Humidity calibration data
    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t dig_H6;

    // t_fine carries fine temperature as global value for compensation methods
    int32_t t_fine;

    bool initialized;

    void readCalibraionData();
    void writeConfig();

    bool checkChipId(byte chipId);

    byte getChipId();
};

#include "bme280.tpp"