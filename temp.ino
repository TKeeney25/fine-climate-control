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

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(DIGIT_SERIAL_PIN, OUTPUT);
  pinMode(DIGIT_REGISTER_CLOCK_PIN, OUTPUT);
  pinMode(DIGIT_SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
  pinMode(DIGIT_SHIFT_REGISTER_CLEAR_PIN, OUTPUT);

  pinMode(DISPLAY_OUTPUT_ENABLE_PIN, OUTPUT);
  pinMode(DISPLAY_REGISTER_CLOCK_PIN, OUTPUT);
  pinMode(DISPLAY_SHIFT_REGISTER_CLOCK_PIN, OUTPUT);
  pinMode(DISPLAY_SHIFT_REGISTER_CLEAR_PIN, OUTPUT);

  digitalWrite(DIGIT_SHIFT_REGISTER_CLEAR_PIN, HIGH);
  digitalWrite(DISPLAY_SHIFT_REGISTER_CLEAR_PIN, HIGH);

  for(int i = 0; i < 9; i++) {
    digitalWrite(DIGIT_SERIAL_PIN, HIGH);
    delay(1);
    digitalWrite(DIGIT_REGISTER_CLOCK_PIN, HIGH);
    digitalWrite(DISPLAY_REGISTER_CLOCK_PIN, HIGH);
    delay(1);
    digitalWrite(DIGIT_REGISTER_CLOCK_PIN, LOW);
    digitalWrite(DISPLAY_REGISTER_CLOCK_PIN, LOW);
    delay(1);
    digitalWrite(DIGIT_SHIFT_REGISTER_CLOCK_PIN, HIGH);
    digitalWrite(DISPLAY_SHIFT_REGISTER_CLOCK_PIN, HIGH);
    delay(1);
    digitalWrite(DIGIT_SHIFT_REGISTER_CLOCK_PIN, LOW);
    digitalWrite(DISPLAY_SHIFT_REGISTER_CLOCK_PIN, LOW);
    delay(1);
  }
  digitalWrite(DIGIT_SERIAL_PIN, LOW);
  Serial.println("DONE");

}

void loop() {

  // put your main code here, to run repeatedly:

}