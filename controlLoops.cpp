#include "controlLoops.h"

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
#pragma region EXTERNS
unsigned long lastTemperatureControl = 0;
unsigned long lastDisplayCycle = 0;
#pragma endregion

#pragma region SIMPLE
void controlTemperatureSimple(float currTemp, float maxTemp, float minTemp) {
    if (lastTemperatureControl < millis()) {
        if (currTemp >= maxTemp) {
            digitalWrite(COOLER_CONTROL_PIN, HIGH);
            lastTemperatureControl = millis() + MILLIS_BETWEEN_CONTROLS;
            lastDisplayCycle = millis();
        } else {
            digitalWrite(COOLER_CONTROL_PIN, LOW);
        }
        if (currTemp <= minTemp) {
            digitalWrite(HEATER_CONTROL_PIN, HIGH);
            lastTemperatureControl = millis() + MILLIS_BETWEEN_CONTROLS;
            lastDisplayCycle = millis();
        } else {
            digitalWrite(HEATER_CONTROL_PIN, LOW);
        }
    }
}
#pragma endregion

#pragma region PID
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

    float p = currTemp * KP;
    i = i + (currTemp * KI) * timeDelta;
    d = KD*(currTemp - previousTemp)/timeDelta;

    float pid = p + i + d;
    if (pid >= 1) {
        digitalWrite(COOLER_CONTROL_PIN, HIGH);
    } else {
        digitalWrite(COOLER_CONTROL_PIN, LOW);
    }
    if (pid <= 1) {
        digitalWrite(HEATER_CONTROL_PIN, HIGH);
    } else {
        digitalWrite(HEATER_CONTROL_PIN, LOW);
    }
    previousTemp = currTemp;
}
#pragma endregion