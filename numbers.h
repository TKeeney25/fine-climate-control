#include <Arduino.h>
// A,B,C,D,E,F,G
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
struct sevenSegmentDisplayPanel {
    uint8_t char_1;
    bool char_1_decimal;
    uint8_t char_1_position;
    uint8_t char_2;
    bool char_2_decimal;
    uint8_t char_2_position;
    uint8_t char_3;
    bool char_3_decimal;
    uint8_t char_3_position;
};