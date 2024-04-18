#include "displays.h"

displayGroup minTemperatureDisplay = {
    0, 1, 2
};
displayGroup maxTemperatureDisplay = {
    0, 3, 4
};
displayGroup currTemperatureDisplay = {
    5, 6, 7
};

void setup() {
    Serial.begin(9600);
    displaySetup();
}
unsigned long lastTime = millis();
void loop() {
    if (millis() < lastTime || millis() >= lastTime + 200) {
        cycleDisplay();
        lastTime = millis();
    }
    updateDisplayGroup(minTemperatureDisplay, 12.0);
    updateDisplayGroup(maxTemperatureDisplay, 34.0);
    updateDisplayGroup(currTemperatureDisplay, 56.7);
}