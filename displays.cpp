#include "displays.h"

uint8_t activeDisplays = DEFAULT_ACTIVE_DISPLAYS; // Whichever display has 1 is active
uint8_t currentDisplay = DEFAULT_CURRENT_DISPLAY;

sevenSegmentDisplay displays[8] = {
    {CLEAR, true},
    {CLEAR, false},
    {CLEAR, false},
    {CLEAR, true},
    {CLEAR, false},
    {CLEAR, false}, 
    {CLEAR, true},
    {CLEAR, false}
};

void displaySetup() {
    pinMode(DIGIT_SERIAL_PIN, OUTPUT);
    pinMode(DIGIT_REGISTER_CLOCK_PIN, OUTPUT);
    pinMode(DIGIT_SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
    pinMode(DIGIT_SHIFT_REGISTER_CLEAR_PIN, OUTPUT);

    pinMode(DISPLAY_SERIAL_PIN, OUTPUT);
    pinMode(DISPLAY_REGISTER_CLOCK_PIN, OUTPUT);
    pinMode(DISPLAY_SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
    pinMode(DISPLAY_SHIFT_REGISTER_CLEAR_PIN, OUTPUT);
    pinMode(DISPLAY_OUTPUT_ENABLE_PIN, OUTPUT);

    digitalWrite(DIGIT_SHIFT_REGISTER_CLEAR_PIN, LOW);
    digitalWrite(DISPLAY_SHIFT_REGISTER_CLEAR_PIN, LOW);
    delay(1);
    digitalWrite(DIGIT_SHIFT_REGISTER_CLEAR_PIN, HIGH);
    digitalWrite(DISPLAY_SHIFT_REGISTER_CLEAR_PIN, HIGH);
}

uint8_t floatMap(char character) {
    if (character == '.') {
        return DECIMAL;
    } else if (character == '0') {
        return ZERO;
    } else if (character == '1') {
        return ONE;
    } else if (character == '2') {
        return TWO;
    } else if (character == '3') {
        return THREE;
    } else if (character == '4') {
        return FOUR;
    } else if (character == '5') {
        return FIVE;
    } else if (character == '6') {
        return SIX;
    } else if (character == '7') {
        return SEVEN;
    } else if (character == '8') {
        return EIGHT;
    } else if (character == '9') {
        return NINE;
    } else {
        return CLEAR;
    }
}

void updateDisplayGroup(displayGroup group, float value) {
    if (value < 0) {
      value = 0.0;
    }
    if (value > 99) {
      value = 99.0;
    }
    char buffer[4];
    dtostrf(value, 3, 1, buffer);
    int current = 1;
    for(int i = 0; i < 4; i++) {
        uint8_t mapping = floatMap(buffer[i]);
        if (mapping == DECIMAL) {
            continue;
        }
        if (current == 1) {
            displays[group.third].character = mapping;
        } else if (current == 2) {
            displays[group.second].character = mapping;
        } else if (current == 3) {
            displays[group.first].character = mapping;
        }
        current += 1;
    }
    Serial.println(displays[group.first].character); // Removing breaks code
}

void displayDigit(uint8_t bits, bool decimal) {
    if (decimal) {
        bits = bits | DECIMAL;
    }
    const uint8_t bitmask = 0b00000001;
    for (int i = 0; i < 8; i++) {
    if (bits & bitmask) {
        digitalWrite(DIGIT_SERIAL_PIN, HIGH);
    } else {
        digitalWrite(DIGIT_SERIAL_PIN, LOW);
    }
    bits = (bits >> 1);
    digitalWrite(DIGIT_SHIFT_REGISTER_CLOCK_PIN, HIGH);
    digitalWrite(DIGIT_SHIFT_REGISTER_CLOCK_PIN, LOW);
  }
  digitalWrite(DIGIT_REGISTER_CLOCK_PIN, HIGH);
  digitalWrite(DIGIT_REGISTER_CLOCK_PIN, LOW);
}

void enableDisplay(bool enable) {
    if(enable) {
        digitalWrite(DISPLAY_OUTPUT_ENABLE_PIN, HIGH);
    } else {
        digitalWrite(DISPLAY_OUTPUT_ENABLE_PIN, LOW);
    }
}

void nextDisplay() {
    if(currentDisplay == 0) {
        digitalWrite(DISPLAY_SERIAL_PIN, LOW);
    } else {
        digitalWrite(DISPLAY_SERIAL_PIN, HIGH);
    }
    digitalWrite(DISPLAY_SHIFT_REGISTER_CLOCK_PIN, HIGH);
    digitalWrite(DISPLAY_SHIFT_REGISTER_CLOCK_PIN, LOW);
    digitalWrite(DISPLAY_REGISTER_CLOCK_PIN, HIGH);
    digitalWrite(DISPLAY_REGISTER_CLOCK_PIN, LOW);
}

void turnOffDisplays() {
    displayDigit(0, false);
}

void cycleDisplay() {
    enableDisplay(false);
    if (activeDisplays == 0) {
        activeDisplays = DEFAULT_ACTIVE_DISPLAYS;
        currentDisplay = DEFAULT_CURRENT_DISPLAY;
    }
    uint8_t displayValue = displays[currentDisplay].character;
    uint8_t displayDecimal = displays[currentDisplay].decimal;
    nextDisplay();
    displayDigit(displayValue, displayDecimal);
    activeDisplays = activeDisplays << 1;
    currentDisplay++;
    enableDisplay(true);
}