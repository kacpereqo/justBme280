#include <Wire.h>
#include "registers.hpp"
#include "utils/i2c.hpp"

byte addr = 0x76;

enum CONFIG_MODE : uint8_t
{
  SLEEP = 0b00,
  FORCED = 0b01,
  NORMAL = 0b11
};

enum OVERSAMPLING : uint8_t
{

  SKIP = 0b000,
  OVERSAMPLING_1 = 0b001,
  OVERSAMPLING_2 = 0b010,
  OVERSAMPLING_4 = 0b011,
  OVERSAMPLING_8 = 0b100,
  OVERSAMPLING_16 = 0b101
};

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

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  while (!Serial)
    ;

  byte ctrl_meas = (OVERSAMPLING::OVERSAMPLING_1 << 5) | (OVERSAMPLING::OVERSAMPLING_1 << 2) | CONFIG_MODE::NORMAL;

  I2C::writeRegister(addr, BME280_CONFIG_REG, 0b00100101);
  I2C::writeRegister(addr, BME280_CTRL_HUMIDITY_REG, OVERSAMPLING::OVERSAMPLING_1);
  I2C::writeRegister(addr, BME280_CTRL_MEAS_REG, ctrl_meas);

  dig_T1 = I2C::readRegister<uint16_t>(addr, BME280_DIG_T1_LSB_REG);
  dig_T2 = I2C::readRegister<int16_t>(addr, BME280_DIG_T2_LSB_REG);
  dig_T3 = I2C::readRegister<int16_t>(addr, BME280_DIG_T3_LSB_REG);

  dig_P1 = I2C::readRegister<uint16_t>(addr, BME280_DIG_P1_LSB_REG);
  dig_P2 = I2C::readRegister<int16_t>(addr, BME280_DIG_P2_LSB_REG);
  dig_P3 = I2C::readRegister<int16_t>(addr, BME280_DIG_P3_LSB_REG);
  dig_P4 = I2C::readRegister<int16_t>(addr, BME280_DIG_P4_LSB_REG);
  dig_P5 = I2C::readRegister<int16_t>(addr, BME280_DIG_P5_LSB_REG);
  dig_P6 = I2C::readRegister<int16_t>(addr, BME280_DIG_P6_LSB_REG);
  dig_P7 = I2C::readRegister<int16_t>(addr, BME280_DIG_P7_LSB_REG);
  dig_P8 = I2C::readRegister<int16_t>(addr, BME280_DIG_P8_LSB_REG);
  dig_P9 = I2C::readRegister<int16_t>(addr, BME280_DIG_P9_LSB_REG);

  dig_H1 = I2C::readRegister<uint8_t>(addr, BME280_DIG_H1_REG);
  dig_H2 = I2C::readRegister<int16_t>(addr, BME280_DIG_H2_LSB_REG);
  dig_H3 = I2C::readRegister<uint8_t>(addr, BME280_DIG_H3_REG);
  dig_H4 = (I2C::readRegister<int8_t>(addr, BME280_DIG_H4_MSB_REG) << 4) | (I2C::readRegister<int8_t>(addr, BME280_DIG_H4_LSB_REG) & 0x0F);
  dig_H5 = (I2C::readRegister<int8_t>(addr, BME280_DIG_H5_MSB_REG) << 4) | (I2C::readRegister<int8_t>(addr, BME280_DIG_H5_MSB_REG) >> 4);
  dig_H6 = I2C::readRegister<int8_t>(addr, BME280_DIG_H6_REG);
}

int32_t t_fine;

int32_t compensateTemperatureRaw(int32_t adc_T)
{
  int32_t var1, var2, T;

  var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
  var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;

  t_fine = var1 + var2;
  T = (t_fine * 5 + 128) >> 8;

  return T;
}

// 4.2.3 Compensation formula in 64 bit arithmetic
int32_t compensatePressureRaw(int32_t adc_P)
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

float compensateTemperatureFloat(int32_t adc_T)
{
  return compensateTemperatureRaw(adc_T) / 100.0f;
}

uint32_t compensateHumidity(int32_t adc_H)
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

struct BME280rawData
{
  byte pressure[3];
  byte temperature[3];
  byte humidity[2];
};

void loop()
{
  // burst read raw data from the sensor 0xF7 - 0xFE
  BME280rawData readData;
  readData = I2C::readRegister<BME280rawData>(addr, BME280_PRESSURE_MSB_REG);

  // analog / digital converter raw values
  int32_t adc_T = (readData.temperature[0] << 12) | (readData.temperature[1] << 4) | (readData.temperature[2] >> 4);
  int32_t adc_P = (readData.pressure[0] << 12) | (readData.pressure[1] << 4) | (readData.pressure[2] >> 4);
  int32_t adc_H = (readData.humidity[0] << 8) | readData.humidity[1];

  Serial.print(compensateTemperatureFloat(adc_T));
  Serial.print("°C ");
  Serial.print(compensatePressureRaw(adc_P) / 256.0f / 100.0f);
  Serial.print("hPa ");
  Serial.print(compensateHumidity(adc_H) / 1024.0f);
  Serial.print("%");

  // uint32_t raw_pressure = (readData[0] << 12) | (readData[1] << 4) | (readData[2] >> 4);
  // uint32_t raw_temperature = (readData[3] << 12) | (readData[4] << 4) | (readData[5] >> 4);

  // Serial.print(readData.pressure[0], HEX);
  // Serial.print("|");
  // Serial.print(readData.pressure[1], HEX);
  // Serial.print("|");
  // Serial.print(readData.pressure[2], HEX);
  // Serial.print(" ");
  // // Serial.print(readData.temperature[0], HEX);
  // // Serial.print(readData.temperature[1], HEX);
  // // Serial.print(readData.temperature[2], HEX);
  // // Serial.print(" ");

  // Serial.print(readData.humidity[0], HEX);
  // Serial.print(readData.humidity[1], HEX);
  Serial.println();

  delay(1000.0f / 1.0f);
}