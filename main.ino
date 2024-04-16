#include "numbers.h"
#include <OneWire.h>
#include <TimerOne.h>
#include <DallasTemperature.h>

#pragma region TEMPERATURE CONTROL
#define TEMPERATURE_SENSOR_PIN 6
#define TEMPERATURE_CONTROL_PIN 5
#define HEATER_CONTROL_PIN 4
#define PING_DELAY 1000000
OneWire oneWire(TEMPERATURE_SENSOR_PIN);
DallasTemperature sensors(&oneWire);
#pragma endregion

#pragma region DIGIT CONTROL
#define DIGIT_SERIAL_PIN 13
#define DIGIT_REGISTER_CLOCK_PIN 12
#define DIGIT_SHIFT_REGISTER_CLOCK_PIN 11
#define DIGIT_SHIFT_REGISTER_CLEAR_PIN 10
#pragma endregion

#pragma region DISPLAY CONTROL
#define DISPLAY_CYCLES_PER_SECOND 100
#define DISPLAY_SERIAL_PIN 13
#define DISPLAY_OUTPUT_ENABLE_PIN 6
#define DISPLAY_REGISTER_CLOCK_PIN 9
#define DISPLAY_SHIFT_REGISTER_CLOCK_PIN 8
#define DISPLAY_SHIFT_REGISTER_CLEAR_PIN 7
#pragma endregion

#pragma region POTENTIOMETER CONTROL
#define MIN_TEMPERATURE_POT A8
#define MAX_TEMPERATURE_POT A7
#define MAX_POT_VALUE 1023
#define MIN_POT_VALUE 0
#define MAX_TEMPERATURE 90
#define MIN_TEMPERATURE 50
#define POT_STEP ((int)(MAX_POT_VALUE - MIN_POT_VALUE) / (MAX_TEMPERATURE - MIN_TEMPERATURE))
#define MAX_OFFSET_TEMPERATURE 25
#define MIN_OFFSET_TEMPERATURE 5
#pragma endregion

#define BUTTON_PIN A6
#define TIME_TIL_SLEEP_MS 60000

bool awake = true;
unsigned long last_interaction = millis();
#pragma region 7 SEGMENT DISPLAYS
sevenSegmentDisplayPanel min_temperature_display = {
  ZERO, false, 0b00000001,
  ONE, true, 0b00000010,
  TWO, false, 0b00000100
};
sevenSegmentDisplayPanel max_temperature_display = {
  ZERO, false, 0b00000001,
  THREE, true, 0b00001000,
  FOUR, false, 0b00010000
};
sevenSegmentDisplayPanel current_temperature_display = {
  FIVE, false, 0b00100000,
  SIX, false, 0b01000000,
  SEVEN, false, 0b10000000
};
#pragma endregion

#pragma region Pointers
uint8_t *temperature_characters[8] = {
  &min_temperature_display.char_1,
  &min_temperature_display.char_2,
  &min_temperature_display.char_3,
  &max_temperature_display.char_2,
  &max_temperature_display.char_3,
  &current_temperature_display.char_1,
  &current_temperature_display.char_2,
  &current_temperature_display.char_3
};
bool *decimal_presence[8] = {
  &min_temperature_display.char_1_decimal,
  &min_temperature_display.char_2_decimal,
  &min_temperature_display.char_3_decimal,
  &max_temperature_display.char_2_decimal,
  &max_temperature_display.char_3_decimal,
  &current_temperature_display.char_1_decimal,
  &current_temperature_display.char_2_decimal,
  &current_temperature_display.char_3_decimal
};
#pragma endregion

void setup() {
  Serial.begin(9600);

  //Timer1.initialize(PING_DELAY);
  //Timer1.attachInterrupt(get_temperature_and_update, PING_DELAY);
  //Timer1.attachInterrupt(cycle_display, 1000000 / DISPLAY_CYCLES_PER_SECOND);
  
  pinMode(TEMPERATURE_SENSOR_PIN, INPUT);
  pinMode(TEMPERATURE_CONTROL_PIN, OUTPUT);
  pinMode(HEATER_CONTROL_PIN, OUTPUT);

  pinMode(DIGIT_SERIAL_PIN, OUTPUT);
  pinMode(DIGIT_REGISTER_CLOCK_PIN, OUTPUT);
  pinMode(DIGIT_SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
  pinMode(DIGIT_SHIFT_REGISTER_CLEAR_PIN, OUTPUT);

  pinMode(DISPLAY_SERIAL_PIN, OUTPUT);
  pinMode(DISPLAY_REGISTER_CLOCK_PIN, OUTPUT);
  pinMode(DISPLAY_SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
  pinMode(DISPLAY_SHIFT_REGISTER_CLEAR_PIN, OUTPUT);
  pinMode(DISPLAY_OUTPUT_ENABLE_PIN, OUTPUT);

  pinMode(MIN_TEMPERATURE_POT, INPUT);
  pinMode(MAX_TEMPERATURE_POT, INPUT);

  pinMode(BUTTON_PIN, INPUT);
  cycle_display();
}

// Maps characters to bitmaps
uint8_t float_map(char character) {
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

#pragma region DISPLAY CONTROL
uint8_t active_displays = 0b00000001;
uint8_t curr_display = 0;
void cycle_display() {
  awake = true;
  if (!awake) {
    enable_display(false);
    return;
  }
  if (curr_display > 7) {
    active_displays = 0b00000001;
    curr_display = 0;
  }
  uint8_t display_value = *temperature_characters[curr_display];
  enable_display(false);
  display_digit(display_value, *decimal_presence[curr_display]);
  Serial.print(curr_display);
  Serial.print(": ");
  Serial.print( `);
  Serial.print(".");
  Serial.println(*decimal_presence[curr_display]);
  next_display();
  enable_display(true);
  active_displays = active_displays << 1;
  curr_display++;
}
void display_digit(uint8_t bits, bool decimal) {
  digitalWrite(DIGIT_SHIFT_REGISTER_CLEAR_PIN, HIGH);
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
  digitalWrite(DIGIT_SHIFT_REGISTER_CLEAR_PIN, LOW);
}
void enable_display(bool enable) {
  if(enable) {
    digitalWrite(DISPLAY_OUTPUT_ENABLE_PIN, LOW);
  } else {
    digitalWrite(DISPLAY_OUTPUT_ENABLE_PIN, HIGH);
  }
}
void next_display() {
  digitalWrite(DISPLAY_SHIFT_REGISTER_CLEAR_PIN, HIGH); // ALWAYS LOW
  if(curr_display == 0) {
    digitalWrite(DISPLAY_SERIAL_PIN, LOW);
  } else {
    digitalWrite(DISPLAY_SERIAL_PIN, LOW);
  }
  digitalWrite(DISPLAY_SHIFT_REGISTER_CLOCK_PIN, HIGH);
  digitalWrite(DISPLAY_SHIFT_REGISTER_CLOCK_PIN, LOW);
  digitalWrite(DISPLAY_REGISTER_CLOCK_PIN, HIGH);
  digitalWrite(DISPLAY_REGISTER_CLOCK_PIN, LOW);
}
void write_float_to_display(float value, sevenSegmentDisplayPanel display) {
  value = floor(value + 0.5);
  char buffer[4];
  snprintf(buffer, sizeof buffer, "%f", value);
  int current_display = 1;
  for(int i = 0; i < 4; i++) {
    uint8_t mapping = float_map(buffer[i]);
    if (mapping == DECIMAL) {
      continue;
    }
    if (current_display == 1) {
      display.char_1 = mapping;
    } else if (current_display == 2)
    {
      display.char_2 = mapping;
    } else if (current_display == 3) {
      display.char_3 = mapping;
    }
    current_display += 1;
  }
}
#pragma endregion

#pragma region TEMPERATURE CONTROL
float get_current_temperature() {
  sensors.requestTemperatures(); 
  return sensors.getTempFByIndex(0);
}
void get_temperature_and_update(int maximum, int minimum) {
  float current_temperature = get_current_temperature();
  write_float_to_display(current_temperature, current_temperature_display);
  if (current_temperature >= maximum) {
    digitalWrite(TEMPERATURE_CONTROL_PIN, HIGH);
  } else {
    digitalWrite(TEMPERATURE_CONTROL_PIN, LOW);
  }
  if (current_temperature <= minimum) {
    digitalWrite(HEATER_CONTROL_PIN, HIGH);
  } else {
    digitalWrite(HEATER_CONTROL_PIN, LOW);
  }
}
#pragma endregion

int map_potentiometer_to_range(int reading, int min, int max){
  return map(reading, MIN_POT_VALUE, MAX_POT_VALUE, min, max);
}
int last_max = 0;
int last_min = 0;
void loop() {
  for(int i = 0; i < 20; i++) {
    cycle_display();
    delay(100);
  }
  while(true){
    cycle_display();
    delay(1000);
  /*display_digit(EIGHT, true);
  delay(1000);
  display_digit(CLEAR, false);
  delay(1000);*/
  }
  bool pot_changed = false;
  int potentiometer_max = analogRead(MAX_TEMPERATURE_POT);
  if (abs(potentiometer_max - last_max) < POT_STEP) {
    potentiometer_max = last_max;
  } else {
    last_max = potentiometer_max;
    pot_changed = true;
  }

  int potentiometer_min = analogRead(MIN_TEMPERATURE_POT);
  if (abs(potentiometer_min - last_min) < POT_STEP) {
    potentiometer_min = last_min;
  } else {
    last_min = potentiometer_min;
    pot_changed = true;
  }

  int button_state = analogRead(BUTTON_PIN);
  if (!awake && (button_state == 1 || pot_changed)) {
    noInterrupts();
    last_interaction = millis();
    awake = true;
    interrupts();
  } else {
    if (awake && (last_interaction + TIME_TIL_SLEEP_MS > millis() || millis() < last_interaction)) {
      noInterrupts();
      awake = false;
      interrupts();
    }
  }
  int maximum = map_potentiometer_to_range(MAX_POT_VALUE - potentiometer_max, MIN_TEMPERATURE, MAX_TEMPERATURE);
  int minimum = maximum - map_potentiometer_to_range(potentiometer_min, MIN_OFFSET_TEMPERATURE, MAX_OFFSET_TEMPERATURE);
  write_float_to_display((float) maximum, max_temperature_display);
  write_float_to_display((float) minimum, min_temperature_display);
  get_temperature_and_update(maximum, minimum);
}