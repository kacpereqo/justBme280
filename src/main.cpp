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

  // delay(1000.0f / 1.0f);
}