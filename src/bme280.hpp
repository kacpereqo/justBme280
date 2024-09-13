#pragma once

#include "cstdint"
#include "Wire.h"
#include "utils/i2c.hpp"
#include "registers.hpp"

struct BME280
{
public:
    struct rawData
    {
        byte pressure[3];
        byte temperature[3];
        byte humidity[2];
    };

    // Table 27: t_sb settings
    enum class Standby : uint8_t
    {
        STANDBY_0_5 = 0b000,
        STANDBY_62_5 = 0b001,
        STANDBY_125 = 0b010,
        STANDBY_250 = 0b011,
        STANDBY_500 = 0b100,
        STANDBY_1000 = 0b101,
        STANDBY_10 = 0b110,
        STANDBY_20 = 0b111
    };

    // Table 28: filter settings
    enum class Filter : uint8_t
    {
        FILTER_OFF = 0b000,
        FILTER_2 = 0b001,
        FILTER_4 = 0b010,
        FILTER_8 = 0b011,
        FILTER_16 = 0b100
    };

    // Table 20: register settings osrs_h
    enum class Oversampling : uint8_t
    {
        OVERSAMPLING_SKIP = 0b000,
        OVERSAMPLING_1 = 0b001,
        OVERSAMPLING_2 = 0b010,
        OVERSAMPLING_4 = 0b011,
        OVERSAMPLING_8 = 0b100,
        OVERSAMPLING_16 = 0b101
    };

    // Table 25: register settings
    enum class Mode : uint8_t
    {
        SLEEP = 0b00,
        FORCED = 0b01,
        NORMAL = 0b11
    };

    struct Config
    {
        Standby standby = BME280::Standby::STANDBY_0_5;
        Filter filter = BME280::Filter::FILTER_OFF;
        bool spi_3 = false;

        Oversampling temperature_oversampling = BME280::Oversampling::OVERSAMPLING_1;
        Oversampling pressure_oversampling = BME280::Oversampling::OVERSAMPLING_1;
        Oversampling humidity_oversampling = BME280::Oversampling::OVERSAMPLING_1;

        Mode mode = BME280::Mode::NORMAL;
        uint16_t addr = 0x76;
    };

    Config config;

    int32_t adc_T;
    int32_t adc_P;
    int32_t adc_H;

    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;

    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;

    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t dig_H6;

    int32_t t_fine;

    void readCalibraionData();
    void writeConfig();

    void setConfig(Config config);
    byte getChipId();

    bool begin();

    int32_t getTemperatureRaw(int32_t adc_T);
    int32_t getPressureRaw(int32_t adc_P);
    uint32_t getHumidityRaw(int32_t adc_H);

    void update();

    float getTemperature();
    float getPressure();
    float getHumidity();
};