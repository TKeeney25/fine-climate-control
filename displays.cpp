#include "displays.h"

uint8_t activeDisplays = DEFAULT_ACTIVE_DISPLAYS; // Whichever display has 1 is active
uint8_t currentDisplay = DEFAULT_CURRENT_DISPLAY;

sevenSegmentDisplay displays[8] = {
    ZERO, false,
    ONE, true,
    TWO, false,
    THREE, false,
    FOUR, true,
    FIVE, false, 
    SIX, false,
};

void displaySetup() {
    pinMode(SERIAL_PIN, OUTPUT);
    pinMode(DIGIT_REGISTER_CLOCK_PIN, OUTPUT);
    pinMode(DIGIT_SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
    pinMode(DIGIT_SHIFT_REGISTER_CLEAR_PIN, OUTPUT);

    pinMode(DISPLAY_REGISTER_CLOCK_PIN, OUTPUT);
    pinMode(DISPLAY_SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
    pinMode(DISPLAY_SHIFT_REGISTER_CLEAR_PIN, OUTPUT);
    pinMode(DISPLAY_OUTPUT_ENABLE_PIN, OUTPUT);

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
    char buffer[4];
    snprintf(buffer, sizeof buffer, "%f", value);
    uint8_t current = 1;
    for(int i = 0; i < 4; i++) {
        uint8_t mapping = floatMap(buffer[i]);
        if (mapping == DECIMAL) {
            continue;
        }
        if (current == 1) {
            group.first = mapping;
        } else if (current == 2)
        {
            group.second = mapping;
        } else if (current == 3) {
            group.third = mapping;
        }
        current += 1;
    }
}
void updateDisplay(int displayNum, uint8_t character) {
    displays[displayNum].character = character;
}
/*
void displayDigit(uint8_t bits, bool decimal) {
  if (decimal) {
    bits = bits | DECIMAL;
  }
  const uint8_t bitmask = 0b00000001;
  for (int i = 0; i < 8; i++) {
    if (bits & bitmask) {
      digitalWrite(SERIAL_PIN, HIGH);
    } else {
      digitalWrite(SERIAL_PIN, HIGH);
    }
    bits = (bits >> 1);
    digitalWrite(DIGIT_SHIFT_REGISTER_CLOCK_PIN, HIGH);
    digitalWrite(DIGIT_SHIFT_REGISTER_CLOCK_PIN, LOW);
  }
  digitalWrite(DIGIT_REGISTER_CLOCK_PIN, HIGH);
  digitalWrite(DIGIT_REGISTER_CLOCK_PIN, LOW);
}*/

void enableDisplay(bool enable) {
  if(enable) {
    digitalWrite(DISPLAY_OUTPUT_ENABLE_PIN, LOW);
  } else {
    digitalWrite(DISPLAY_OUTPUT_ENABLE_PIN, HIGH);
  }
}
void displayDigit(uint8_t bits, bool decimal) {
    if(currentDisplay == 0) {
        digitalWrite(SERIAL_PIN, HIGH);
    } else {
        digitalWrite(SERIAL_PIN, HIGH);
    }
    digitalWrite(DIGIT_SHIFT_REGISTER_CLOCK_PIN, HIGH);
    digitalWrite(DIGIT_SHIFT_REGISTER_CLOCK_PIN, LOW);
    digitalWrite(DIGIT_REGISTER_CLOCK_PIN, HIGH);
    digitalWrite(DIGIT_REGISTER_CLOCK_PIN, LOW);
}
void nextDisplay() {
    if(currentDisplay == 0) {
        digitalWrite(SERIAL_PIN, LOW);
    } else {
        digitalWrite(SERIAL_PIN, LOW);
    }
    digitalWrite(DISPLAY_SHIFT_REGISTER_CLOCK_PIN, HIGH);
    digitalWrite(DISPLAY_SHIFT_REGISTER_CLOCK_PIN, LOW);
    digitalWrite(DISPLAY_REGISTER_CLOCK_PIN, HIGH);
    digitalWrite(DISPLAY_REGISTER_CLOCK_PIN, LOW);
}

void cycleDisplay() {
    if (activeDisplays == 0) {
        activeDisplays = DEFAULT_ACTIVE_DISPLAYS;
        currentDisplay = DEFAULT_CURRENT_DISPLAY;
    }
    uint8_t displayValue = displays[currentDisplay].character;
    uint8_t displayDecimal= displays[currentDisplay].decimal;
    enableDisplay(true);
    displayDigit(displayValue, displayDecimal);
    //updateDisplay(displayValue, displayDecimal);
    nextDisplay();
    enableDisplay(true);
    activeDisplays = activeDisplays << 1;
    currentDisplay++;
}