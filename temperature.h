#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>


#define TEMPERATURE_SENSOR_PIN 6
#define TEMPERATURE_CONTROL_PIN 5
#define HEATER_CONTROL_PIN 4
#define PING_DELAY 1000000
OneWire oneWire(TEMPERATURE_SENSOR_PIN);
DallasTemperature sensors(&oneWire);