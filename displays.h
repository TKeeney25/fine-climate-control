#pragma once

#include <Arduino.h>

#define SERIAL_PIN 13
#define DIGIT_REGISTER_CLOCK_PIN 12
#define DIGIT_SHIFT_REGISTER_CLOCK_PIN 11
#define DIGIT_SHIFT_REGISTER_CLEAR_PIN 10

#define DISPLAY_REGISTER_CLOCK_PIN 9
#define DISPLAY_SHIFT_REGISTER_CLOCK_PIN 8
#define DISPLAY_SHIFT_REGISTER_CLEAR_PIN 7
#define DISPLAY_OUTPUT_ENABLE_PIN 6

#define DEFAULT_ACTIVE_DISPLAYS 0b00000001
#define DEFAULT_CURRENT_DISPLAY 0

#define CLEAR 0b00000000 // 0
#define DECIMAL 0b00000001 // 1
#define ZERO 0b11111100 // 252
#define ONE 0b01100000 // 96
#define TWO 0b11011010 // 218
#define THREE 0b11110010 // 242
#define FOUR 0b01100110 // 102
#define FIVE 0b10110110 // 182
#define SIX 0b10111110 // 190
#define SEVEN 0b11100000 // 224
#define EIGHT 0b11111110 // 254
#define NINE 0b11110110 // 246


struct sevenSegmentDisplay {
  uint8_t character;
  bool decimal;
};
struct displayGroup {
    uint8_t first;
    uint8_t second;
    uint8_t third;
};

// Initializes display data
void displaySetup();

// Converts a number character into a seven-segment binary representation
uint8_t floatMap(char character);

// Sets the value of a 3 display 7 segment display to a given float
void updateDisplayGroup(displayGroup group, float value);

// Sets the value of a 7 segment display to a given binary representation
void updateDisplay(int displayNum, uint8_t character);

// Disables the displaying of digits to all displays
void enableDisplay(bool enable);

// Disables the current display and activates the next one
void nextDisplay();

// Disables current display, updates digits to be displayed to the next display, and activates the next display
void cycleDisplay();