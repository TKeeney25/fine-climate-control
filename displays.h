#pragma once

#include <Arduino.h>

#define DIGIT_SERIAL_PIN 13
#define DIGIT_REGISTER_CLOCK_PIN 12
#define DIGIT_SHIFT_REGISTER_CLOCK_PIN 11
#define DIGIT_SHIFT_REGISTER_CLEAR_PIN 10

#define DISPLAY_SERIAL_PIN 9
#define DISPLAY_REGISTER_CLOCK_PIN 8
#define DISPLAY_OUTPUT_ENABLE_PIN 7
#define DISPLAY_SHIFT_REGISTER_CLOCK_PIN 6
#define DISPLAY_SHIFT_REGISTER_CLEAR_PIN 5

#define DEFAULT_ACTIVE_DISPLAYS 0b00000001
#define DEFAULT_CURRENT_DISPLAY 0

// A,F,B,D,E,DP,C,G
#define CLEAR 0b00000000 // 0
#define DECIMAL 0b00000100 // 
#define ZERO 0b11111010 // 
#define ONE 0b00100010 // 
#define TWO 0b10111001 // 
#define THREE 0b10110011 // 
#define FOUR 0b01100011 // 
#define FIVE 0b11010011 // 
#define SIX 0b11011011 // 
#define SEVEN 0b10100010 // 
#define EIGHT 0b11111011 // 
#define NINE 0b11100011 // 


struct sevenSegmentDisplay {
    uint8_t character;
    bool decimal;
};

// Declares where within the sevenSegmentDisplay array the display's data is stored
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

// Stores a given binary representation in the register to be displayed
void displayDigit(uint8_t bits, bool decimal);

// Disables the displaying of digits to all displays from the register
void enableDisplay(bool enable);

// Disables the current display and activates the next one
void nextDisplay();

// Disables current display, updates digits to be displayed to the next display, and activates the next display
void cycleDisplay();