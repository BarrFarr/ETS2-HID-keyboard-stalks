#include <Keyboard.h>

// === ROTARY SWITCH SECTION (A0) ===
unsigned long previousMillis = 0;
const long interval = 1000; // Scan once per second
int lastPosition = 0;

// === DIGITAL BUTTONS SECTION ===

// Input pins
const int pin2 = 2;
const int pin3 = 3;
const int pin4 = 4;
const int pin5 = 5;
const int pin6 = 6;
const int pin7 = 7;
const int pin8 = 8;
const int pin9 = 9;
const int pin10 = 10;
const int pin16 = 16;
const int pin21 = 21;

// Output pins for LEDs
const int ledPinSemicolon = A2;
const int ledPin5 = 11;
const int ledPin8 = 12;
const int ledPin2 = 13;

// Variables for tracking button states
bool pin2_state = false;
bool pin3_state = false;
bool pin4_state = false;
bool pin5_state = false;
bool pin6_state = false;
bool pin7_state = false;
bool pin8_state = false;
bool pin9_state = false;
bool pin10_state = false;
bool pin16_state = false;
bool pin21_state = false;

// Variables for tracking combo states
bool combo_9_6_pressed = false;
bool combo_9_21_pressed = false;
bool combo_2_3_pressed = false;

// Variables for tracking LED states
bool ledState = false;
bool semicolonLedState = false;

void setup() {
  // Serial communication is optional, only for debugging.
  // It can be removed in the final version.
  Serial.begin(9600);

  // <<< THIS IS THE KEY CHANGE >>>
  // The following line was commented out.
  // It caused the program to wait for the Serial Monitor to open in the IDE.
  // Without it, the keyboard starts immediately after connecting.
  // while (!Serial); 
  
  // Start keyboard emulation
  Keyboard.begin(); 
  
  pinMode(pin2, INPUT_PULLUP);
  pinMode(pin3, INPUT_PULLUP);
  pinMode(pin4, INPUT_PULLUP);
  pinMode(pin5, INPUT_PULLUP);
  pinMode(pin6, INPUT_PULLUP);
  pinMode(pin7, INPUT_PULLUP);
  pinMode(pin8, INPUT_PULLUP);
  pinMode(pin9, INPUT_PULLUP);
  pinMode(pin10, INPUT_PULLUP);
  pinMode(pin16, INPUT_PULLUP);
  pinMode(pin21, INPUT_PULLUP);
  
  // Set LED pins as outputs
  pinMode(ledPinSemicolon, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin8, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void loop() {
  // === ROTARY SWITCH SECTION (RUNS IN BACKGROUND ONCE PER SECOND) ===
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    int analogValue = analogRead(A0);
    int currentPosition = 0;

    // Ranges calculated for a fixed 4.65kΩ resistor
    if (analogValue > 275 && analogValue < 320) {        // Target: ~297
      currentPosition = 1;
    } else if (analogValue > 445 && analogValue < 490) { // Target: ~467
      currentPosition = 2;
    } else if (analogValue > 550 && analogValue < 595) { // Target: ~572
      currentPosition = 3;
    } else if (analogValue > 620 && analogValue < 670) { // Target: ~644
      currentPosition = 4;
    }

    if (currentPosition != lastPosition && currentPosition != 0) {
      // Send key pulse depending on new position
      switch (currentPosition) {
        case 1: Keyboard.write('7'); break;
        case 2: Keyboard.write('8'); break;
        case 3: Keyboard.write('9'); break;
        case 4: Keyboard.write('0'); break;
      }
      lastPosition = currentPosition;
    } else if (currentPosition == 0 && lastPosition != 0) {
      lastPosition = 0;
    }
  }

  // === DIGITAL BUTTONS SECTION (RUNS CONSTANTLY) ===
