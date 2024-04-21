#include "displays.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMPERATURE_SENSOR_PIN 6
#define TEMPERATURE_CONTROL_PIN 5
#define HEATER_CONTROL_PIN 4
#define PING_DELAY 1000000
OneWire oneWire(TEMPERATURE_SENSOR_PIN);
DallasTemperature sensors(&oneWire);

#define COOLER_CONTROL_PIN 5
#define HEATER_CONTROL_PIN 4

#define MILLIS_BETWEEN_DISPLAYS 1
#define MILLIS_BETWEEN_CONTROLS 60000

#pragma region POTENTIOMETER CONTROL
#define MIN_TEMPERATURE_POT A8
#define MAX_TEMPERATURE_POT A7
#define BUTTON_PIN A6
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

void setup() {
    Serial.begin(9600);
    pinMode(MIN_TEMPERATURE_POT, INPUT);
    pinMode(COOLER_CONTROL_PIN, OUTPUT);
    pinMode(HEATER_CONTROL_PIN, OUTPUT);
    displaySetup();
}
unsigned long lastDisplayCycle = millis();
unsigned long lastTemperatureControl = millis();

/*  IDEAS FOR CONTROL LOOPS
 *  SIMPLELOOP: If out of spec run. Add a 1 minute timer
 *      to save the compressor
 *  TEMPDELTA: run cooling/heating until some temperature
 *      delta coefficient has been hit (1 means if past max
 *      it must hit min while 0 means if past max it must hit max)
 *  PID-ish: close to a PID loop. PID values are based off of distance
 *      from the middle of the min/max temps. Once the PID hits a
 *      threshold heating/cooling is turned on. - I kinda like this the most
*/

/* 
 * Simple control loop to control temperature
 * If Temperature is out of spec, run cooling/heating until
 * it is in-spec or 1 minute has passed whichever is longer
 */ 
void controlTemperatureSimple(float currTemp, float maxTemp, float minTemp) {
    if (millis() >= lastTemperatureControl + MILLIS_BETWEEN_CONTROLS) {
        if (currTemp >= maxTemp) {
            digitalWrite(COOLER_CONTROL_PIN, HIGH);
            lastTemperatureControl = millis();
        } else {
            digitalWrite(COOLER_CONTROL_PIN, LOW);
        }
        if (currTemp <= maxTemp) {
            digitalWrite(HEATER_CONTROL_PIN, HIGH);
            lastTemperatureControl = millis();
        } else {
            digitalWrite(HEATER_CONTROL_PIN, LOW);
        }
    }
float i = 0;
float d = 0;

unsigned long previousTime = millis();
float previousTemp = 72;

void controlTemperaturePID(float currTemp, float maxTemp, float minTemp) {
    float timeDelta = ((float)(millis() - previousTime)) / 1000.0;
    previousTime = millis();

    /*  This math ensures that error is the percentage towards max/min
     *  away from the midpoint. This means with Kp of 1 it will behave
     *  similar to the simple control loop.
     */
    float targetTemp = (maxTemp + minTemp) / 2;
    float tempRange = (maxTemp - minTemp) / 2;
    float error = (currTemp - targetTemp) / tempRange;

    p = currTemp * KP;
    i = i + (currTemp * KI) * timeDelta;
    d = KD*(currTemp - previousTemp)/timeDelta;

    float pid = p + i + d;
    if (pid >= COOL_THRESHOLD) {
        digitalWrite(COOLER_CONTROL_PIN, HIGH);
    } else {
        digitalWrite(COOLER_CONTROL_PIN, LOW);
    }
    if (pid <= HEAT_THRESHOLD) {
        digitalWrite(HEATER_CONTROL_PIN, HIGH);
    } else {
        digitalWrite(HEATER_CONTROL_PIN, LOW);
    }
    previousTemp = currTemp;
}

int mapPot(int reading, int min, int max){
  return map(reading, MIN_POT_VALUE, MAX_POT_VALUE, min, max);
}
float get_current_temperature() {
  sensors.requestTemperatures(); 
  return sensors.getTempFByIndex(0);
}

float previousMinTemp = 0;
float previousMaxTemp = 0;
float previousCurrTemp = 0;
void loop() {
  Serial.println(analogRead(MIN_TEMPERATURE_POT));
    // TODO add display sleeping for no activity
    if (millis() < lastDisplayCycle || millis() < previousTime) {
        // Handles the edge case for when millis overflow
        lastDisplayCycle = 0;
        previousTime = 0;
    }
    if (millis() >= lastDisplayCycle + MILLIS_BETWEEN_DISPLAYS) {
        cycleDisplay();
        lastDisplayCycle = millis();
    }
    float currTemp = 56.7;
    float minTemp = MIN_TEMPERATURE;
    float maxTemp = MAX_TEMPERATURE;
    
    /*
    int offset = mapPot(analogRead(MIN_TEMPERATURE_POT), MAX_OFFSET_TEMPERATURE, MIN_OFFSET_TEMPERATURE);
    maxTemp = mapPot(analogRead(MAX_TEMPERATURE_POT), MIN_TEMPERATURE, MAX_TEMPERATURE);
    minTemp = maxTemp - offset;
    currTemp = get_current_temperature();
    int tmp = currTemp * 10; // Truncate the decimal
    currTemp = tmp / 10.0;
    int button_state = analogRead(BUTTON_PIN);
    */
    /*
    if (minTemp != previousMinTemp) {
        updateDisplayGroup(minTemperatureDisplay, minTemp);
        previousMinTemp = minTemp;
    }
    if (maxTemp != previousMaxTemp) {
        updateDisplayGroup(maxTemperatureDisplay, maxTemp);
        previousMaxTemp = maxTemp;
    }
    if (currTemp != previousCurrTemp) {
        updateDisplayGroup(currTemperatureDisplay, currTemp);
    }*/
}