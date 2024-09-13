#include <Wire.h>
#include "registers.hpp"
#include "utils/i2c.hpp"
#include "bme280.hpp"

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  while (!Serial)
    ;
}

// 4.2.3 Compensation formula in 64 bit arithmetic

void loop()
{
  // burst read raw data from the sensor 0xF7 - 0xFE
  // analog / digital converter raw values

  static BME280 bme280;

  bme280.update();

  Serial.print(bme280.getPressure());
  Serial.println(" hPa");

  delay(1000.0f / 1.0f);
}