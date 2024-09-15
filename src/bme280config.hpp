#pragma once

#include <cstdint>

namespace BME280Config
{
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

    enum class Communication : uint8_t
    {
        i2c = 0,
        spi = 1,
        spi_3wire = 2,
    };

    struct Config
    {
    public:
        Oversampling temperature_oversampling;
        Oversampling pressure_oversampling;
        Oversampling humidity_oversampling;
        Communication communication;
        Standby standby;
        Filter filter;
        Mode mode;
        uint16_t addr;
        uint8_t cs_pin;

        Config()
        {
            this->temperature_oversampling = BME280Config::Oversampling::oversampling_x1;
            this->pressure_oversampling = BME280Config::Oversampling::oversampling_x1;
            this->humidity_oversampling = BME280Config::Oversampling::oversampling_x1;
            this->standby = BME280Config::Standby::standby_0_5;
            this->filter = BME280Config::Filter::filter_off;
            this->mode = BME280Config::Mode::normal;
            this->communication = BME280Config::Communication::i2c;
            this->addr = 0x76;
            this->cs_pin = 10;
        }
    };
}
