#pragma once

#include <Arduino.h>

#define COOLER_CONTROL_PIN 2
#define HEATER_CONTROL_PIN 4

#define KP 1.0
#define KI 0.001
#define KD 0.0001

#define MILLIS_BETWEEN_CONTROLS 60000

extern unsigned long lastTemperatureControl;
extern unsigned long lastDisplayCycle;

void controlTemperatureSimple(float currTemp, float maxTemp, float minTemp);
void controlTemperaturePID(float currTemp, float maxTemp, float minTemp);