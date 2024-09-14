#pragma once

constexpr uint8_t BME280_DIG_T1_LSB_REG = 0x88;
constexpr uint8_t BME280_DIG_T1_MSB_REG = 0x89;
constexpr uint8_t BME280_DIG_T2_LSB_REG = 0x8A;
constexpr uint8_t BME280_DIG_T2_MSB_REG = 0x8B;
constexpr uint8_t BME280_DIG_T3_LSB_REG = 0x8C;
constexpr uint8_t BME280_DIG_T3_MSB_REG = 0x8D;

constexpr uint8_t BME280_DIG_P1_LSB_REG = 0x8E;
constexpr uint8_t BME280_DIG_P1_MSB_REG = 0x8F;
constexpr uint8_t BME280_DIG_P2_LSB_REG = 0x90;
constexpr uint8_t BME280_DIG_P2_MSB_REG = 0x91;
constexpr uint8_t BME280_DIG_P3_LSB_REG = 0x92;
constexpr uint8_t BME280_DIG_P3_MSB_REG = 0x93;
constexpr uint8_t BME280_DIG_P4_LSB_REG = 0x94;
constexpr uint8_t BME280_DIG_P4_MSB_REG = 0x95;
constexpr uint8_t BME280_DIG_P5_LSB_REG = 0x96;
constexpr uint8_t BME280_DIG_P5_MSB_REG = 0x97;
constexpr uint8_t BME280_DIG_P6_LSB_REG = 0x98;
constexpr uint8_t BME280_DIG_P6_MSB_REG = 0x99;
constexpr uint8_t BME280_DIG_P7_LSB_REG = 0x9A;
constexpr uint8_t BME280_DIG_P7_MSB_REG = 0x9B;
constexpr uint8_t BME280_DIG_P8_LSB_REG = 0x9C;
constexpr uint8_t BME280_DIG_P8_MSB_REG = 0x9D;
constexpr uint8_t BME280_DIG_P9_LSB_REG = 0x9E;
constexpr uint8_t BME280_DIG_P9_MSB_REG = 0x9F;

constexpr uint8_t BME280_DIG_H1_REG = 0xA1;

constexpr uint8_t BME280_CHIP_ID_REG = 0xD0; // Chip ID
constexpr uint8_t BME280_RST_REG = 0xE0;     // Softreset Reg

constexpr uint8_t BME280_DIG_H2_LSB_REG = 0xE1;
constexpr uint8_t BME280_DIG_H2_MSB_REG = 0xE2;
constexpr uint8_t BME280_DIG_H3_REG = 0xE3;
constexpr uint8_t BME280_DIG_H4_MSB_REG = 0xE4;
constexpr uint8_t BME280_DIG_H4_LSB_REG = 0xE5;
constexpr uint8_t BME280_DIG_H5_MSB_REG = 0xE6;
constexpr uint8_t BME280_DIG_H6_REG = 0xE7;

constexpr uint8_t BME280_CTRL_HUMIDITY_REG = 0xF2; // Ctrl Humidity Reg

constexpr uint8_t BME280_STAT_REG = 0xF3; // Status Reg

constexpr uint8_t BME280_CTRL_MEAS_REG = 0xF4; // Ctrl Measure Reg
constexpr uint8_t BME280_CONFIG_REG = 0xF5;    // Configuration Reg

constexpr uint8_t BME280_PRESSURE_MSB_REG = 0xF7;  // Pressure MSB
constexpr uint8_t BME280_PRESSURE_LSB_REG = 0xF8;  // Pressure LSB
constexpr uint8_t BME280_PRESSURE_XLSB_REG = 0xF9; // Pressure XLSB

constexpr uint8_t BME280_TEMPERATURE_MSB_REG = 0xFA;  // Temperature MSB
constexpr uint8_t BME280_TEMPERATURE_LSB_REG = 0xFB;  // Temperature LSB
constexpr uint8_t BME280_TEMPERATURE_XLSB_REG = 0xFC; // Temperature XLSB

constexpr uint8_t BME280_HUMIDITY_MSB_REG = 0xFD; // Humidity MSB
constexpr uint8_t BME280_HUMIDITY_LSB_REG = 0xFE; // Humidity LSB