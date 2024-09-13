#include <Wire.h>
#include "registers.hpp"
#include "utils/i2c.hpp"
#include "bme280.hpp"

static BME280 bme280;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  while (!Serial)
    ;

  BME280::Config config;

  config.temperature_oversampling = BME280::Oversampling::OVERSAMPLING_SKIP;
  config.pressure_oversampling = BME280::Oversampling::OVERSAMPLING_1;
  config.humidity_oversampling = BME280::Oversampling::OVERSAMPLING_SKIP;

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
  Serial.print(" hPa");
  Serial.print(" ");
  Serial.print(bme280.getTemperature());
  Serial.print(" *C");
  Serial.print(" ");
  Serial.print(bme280.getHumidity());
  Serial.print(" %");
  Serial.println();

  delay(1000.0f / 1.0f);
}