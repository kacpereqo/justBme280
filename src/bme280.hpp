#pragma once

#include "cstdint"
#include "Wire.h"
#include "utils/i2c.hpp"
#include "registers.hpp"

struct BME280
{
public:
    // Table 27: t_sb settings
    enum class Standby : uint8_t
    {
        standby_0_5 = 0b000,
        standby_62_5 = 0b001,
        standby_125 = 0b010,
        standby_250 = 0b011,
        standby_500 = 0b100,
        standby_1000 = 0b101,
        standby_10 = 0b110,
        standby_20 = 0b111
    };

    // Table 28: filter settings
    enum class Filter : uint8_t
    {
        filter_off = 0b000,
        filter_x2 = 0b001,
        filter_x4 = 0b010,
        filter_x8 = 0b011,
        filter_x16 = 0b100
    };

    // Table 20: register settings osrs_h
    enum class Oversampling : uint8_t
    {
        oversampling_off = 0b000,
        oversampling_x1 = 0b001,
        oversampling_x2 = 0b010,
        oversampling_x4 = 0b011,
        oversampling_x8 = 0b100,
        oversampling_x16 = 0b101
    };

    // Table 25: register settings
    enum class Mode : uint8_t
    {
        sleep = 0b00,
        forced = 0b01,
        normal = 0b11
    };

    struct Config
    {
        Standby standby = BME280::Standby::standby_0_5;
        Filter filter = BME280::Filter::filter_off;
        bool spi_3 = false;

        Oversampling temperature_oversampling = BME280::Oversampling::oversampling_x1;
        Oversampling pressure_oversampling = BME280::Oversampling::oversampling_x1;
        Oversampling humidity_oversampling = BME280::Oversampling::oversampling_x1;

        Mode mode = BME280::Mode::normal;
        uint16_t addr = 0x76;
    };

    bool begin();

    void setConfig(Config config);
    void update();

    // 4.2.3 Compensation formulas
    int32_t getTemperatureRaw(int32_t adc_T);
    int32_t getPressureRaw(int32_t adc_P);
    uint32_t getHumidityRaw(int32_t adc_H);

    float getTemperature();
    float getPressure();
    float getHumidity();

    Config config;

private:
    struct rawData
    {
        byte pressure[3];
        byte temperature[3];
        byte humidity[2];
    };

    // analog-digital converter raw data
    int32_t adc_T;
    int32_t adc_P;
    int32_t adc_H;

    // #
    // Table 16: Compensation parameter storage, naming and data type
    // #

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

    void readCalibraionData();
    void writeConfig();

    byte getChipId();
};