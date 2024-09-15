#include <Wire.h>
#include "registers.hpp"
#include "utils/i2c.hpp"
#include "bme280.hpp"
#include "utils/spi.hpp"

static BME280 bme280;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;

  BME280Config::Config config;
  config.temperature_oversampling = BME280Config::Oversampling::oversampling_x1;
  config.pressure_oversampling = BME280Config::Oversampling::oversampling_x1;
  config.humidity_oversampling = BME280Config::Oversampling::oversampling_x1;
  config.filter = BME280Config::Filter::filter_off;
  config.communication = BME280Config::Communication::spi;
  bme280.setConfig(config);

  if (bme280.begin())
  {
    Serial.println("BME280 is ready");
  }
}

void loop()
{
  bme280.update();

  Serial.print(bme280.getPressure());
  Serial.print(" hPa ");
  Serial.print(bme280.getTemperature());
  Serial.print(" *C ");
  Serial.print(bme280.getHumidity());
  Serial.print(" % ");
  Serial.println();
}