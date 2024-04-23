#include "displays.h"
#include "controlLoops.h"

#include <OneWire.h>
#include <DallasTemperature.h>
#include <TimerOne.h>


#define KP 1.0
#define KI 0.001
#define KD 0.0001

#define TEMPERATURE_SENSOR_PIN 3
#define SLEEP_DELAY 60000
OneWire oneWire(TEMPERATURE_SENSOR_PIN);
DallasTemperature sensors(&oneWire);

#define COOLER_CONTROL_PIN 2
#define HEATER_CONTROL_PIN 4

#define MILLIS_BETWEEN_CONTROLS 60000

#pragma region POTENTIOMETER CONTROL
#define MIN_TEMPERATURE_POT A8
#define MAX_TEMPERATURE_POT A7
#define BUTTON_PIN A9
#define MAX_POT_VALUE 1023
#define MIN_POT_VALUE 0
#define MAX_TEMPERATURE 90
#define MIN_TEMPERATURE 50
#define POT_STEP (int)((MAX_POT_VALUE - MIN_POT_VALUE) / (MAX_TEMPERATURE - MIN_TEMPERATURE))
#define MAX_OFFSET_TEMPERATURE 25
#define MIN_OFFSET_TEMPERATURE 5
#pragma endregion

displayGroup minTemperatureDisplay = {
    1, 0, 2
};
displayGroup maxTemperatureDisplay = {
    1, 3, 4
};
displayGroup currTemperatureDisplay = {
    5, 6, 7
};

bool awake = true;
bool displays_active = true;

void callCycleDisplay() {
    noInterrupts();
    if (awake) {
        cycleDisplay();
        displays_active = true;
    } else {
        if (displays_active) {
          turnOffDisplays();
          displays_active = false;
        }
    }
    interrupts();
}

void setup() {
    Serial.begin(9600);
    Timer1.initialize(1000);
    Timer1.attachInterrupt(callCycleDisplay);
    Timer1.start();
    pinMode(MIN_TEMPERATURE_POT, INPUT);
    pinMode(COOLER_CONTROL_PIN, OUTPUT);
    pinMode(HEATER_CONTROL_PIN, OUTPUT);
    displaySetup();
}

int mapPot(int reading, int min, int max){
  return map(reading, MIN_POT_VALUE, MAX_POT_VALUE, min, max);
}
float getCurrentTemperature() {
  sensors.requestTemperatures(); 
  return sensors.getTempFByIndex(0);
}
float truncate(float input) {
    int tmp = input * 10; // Truncate the decimal
    input = tmp / 10.0;
    return input;
}

float previousMinTemp = 0;
float previousMaxTemp = 0;
float previousCurrTemp = 0;
unsigned long previousMillis = millis();
void loop() {
    if (millis() < previousMillis) {
        // Handles the edge case for when millis overflow
        lastDisplayCycle = 0;
        lastTemperatureControl = 0;
    }
    previousMillis = millis();
    if (millis() > lastDisplayCycle + SLEEP_DELAY) {
        awake = false;
    } else {
        awake = true;
    }
    float currTemp = 56.0;
    float minTemp = MIN_TEMPERATURE;
    float maxTemp = MAX_TEMPERATURE;
    
    int offset = mapPot(analogRead(MIN_TEMPERATURE_POT), MIN_OFFSET_TEMPERATURE, MAX_OFFSET_TEMPERATURE);
    maxTemp = mapPot(analogRead(MAX_TEMPERATURE_POT), MIN_TEMPERATURE, MAX_TEMPERATURE);
    minTemp = maxTemp - offset;
    currTemp = truncate(getCurrentTemperature());
    int button_state = analogRead(BUTTON_PIN);

    if (minTemp != previousMinTemp) {
        updateDisplayGroup(minTemperatureDisplay, minTemp);
        previousMinTemp = minTemp;
        lastDisplayCycle = millis();
        lastTemperatureControl = 0;
    }
    if (maxTemp != previousMaxTemp) {
        updateDisplayGroup(maxTemperatureDisplay, maxTemp);
        previousMaxTemp = maxTemp;
        lastDisplayCycle = millis();
        lastTemperatureControl = 0;
    }
    if (currTemp != previousCurrTemp) {
        updateDisplayGroup(currTemperatureDisplay, currTemp);
        previousCurrTemp = currTemp;
    }
    controlTemperatureSimple(currTemp, maxTemp, minTemp);
    if (analogRead(BUTTON_PIN) == 0) {
        lastDisplayCycle = millis();
        lastTemperatureControl = 0;
    }
}