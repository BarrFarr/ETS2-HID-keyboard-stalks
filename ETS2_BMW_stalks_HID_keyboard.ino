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

  bool current_pin2_state = (digitalRead(pin2) == LOW);
  bool current_pin3_state = (digitalRead(pin3) == LOW);
  bool current_pin4_state = (digitalRead(pin4) == LOW);
  bool current_pin5_state = (digitalRead(pin5) == LOW);
  bool current_pin6_state = (digitalRead(pin6) == LOW);
  bool current_pin7_state = (digitalRead(pin7) == LOW);
  bool current_pin8_state = (digitalRead(pin8) == LOW);
  bool current_pin9_state = (digitalRead(pin9) == LOW);
  bool current_pin10_state = (digitalRead(pin10) == LOW);
  bool current_pin16_state = (digitalRead(pin16) == LOW);
  bool current_pin21_state = (digitalRead(pin21) == LOW);

  // Rest of the code
  if (current_pin9_state && current_pin6_state) {
    if (!combo_9_6_pressed) {
      Keyboard.press(KEY_KP_5);
      combo_9_6_pressed = true;
      ledState = !ledState;
      digitalWrite(ledPin5, ledState ? HIGH : LOW);
    }
  } else {
    if (combo_9_6_pressed) {
      Keyboard.release(KEY_KP_5);
      combo_9_6_pressed = false;
    }
    if (current_pin6_state && !current_pin9_state && pin9_state) {
      Keyboard.press(KEY_KP_8);
      digitalWrite(ledPin8, HIGH);
    } else if (pin6_state && !current_pin6_state) {
      Keyboard.release(KEY_KP_8);
      digitalWrite(ledPin8, LOW);
    }
  }

  if (current_pin9_state && current_pin21_state) {
    if (!combo_9_21_pressed) {
      Keyboard.press(KEY_KP_2);
      digitalWrite(ledPin2, HIGH);
      combo_9_21_pressed = true;
    }
  } else {
    if (combo_9_21_pressed) {
      Keyboard.release(KEY_KP_2);
      digitalWrite(ledPin2, LOW);
      combo_9_21_pressed = false;
    }
    if (current_pin21_state && !current_pin9_state && pin9_state) {
      Keyboard.press(KEY_KP_0);
    } else if (pin21_state && !current_pin21_state) {
      Keyboard.release(KEY_KP_0);
    }
  }

  if (current_pin2_state && current_pin3_state) {
    if (!combo_2_3_pressed) {
      Keyboard.press('.');
      combo_2_3_pressed = true;
    }
  } else {
    if (combo_2_3_pressed) {
      Keyboard.release('.');
      combo_2_3_pressed = false;
    }
    if (current_pin2_state && !current_pin3_state && pin3_state) {
      Keyboard.press('/');
    } else if (pin2_state && !current_pin2_state) {
      Keyboard.release('/');
    }
  }

  if (current_pin3_state && !current_pin2_state && !pin3_state) { Keyboard.press(','); pin3_state = true; } 
  else if (!current_pin3_state && pin3_state) { Keyboard.release(','); pin3_state = false; }
  
  if (current_pin5_state && !pin5_state) { Keyboard.press(';'); pin5_state = true; } 
  else if (!current_pin5_state && pin5_state) { Keyboard.release(';'); pin5_state = false; }
  
  if (current_pin4_state && !pin4_state) {
    Keyboard.press('\'');
    semicolonLedState = !semicolonLedState;
    digitalWrite(ledPinSemicolon, semicolonLedState ? HIGH : LOW);
    pin4_state = true;
  } else if (!current_pin4_state && pin4_state) {
    Keyboard.release('\'');
    pin4_state = false;
  }
  
  if (current_pin16_state && !pin16_state) { Keyboard.press(KEY_KP_1); pin16_state = true; } 
  else if (!current_pin16_state && pin16_state) { Keyboard.release(KEY_KP_1); pin16_state = false; }
  
  if (current_pin10_state && !pin10_state) { Keyboard.press(KEY_KP_7); pin10_state = true; } 
  else if (!current_pin10_state && pin10_state) { Keyboard.release(KEY_KP_7); pin10_state = false; }

  if (current_pin7_state && !pin7_state) { Keyboard.press(KEY_KP_3); pin7_state = true; } 
  else if (!current_pin7_state && pin7_state) { Keyboard.release(KEY_KP_3); pin7_state = false; }
  
  if (current_pin8_state && !pin8_state) { Keyboard.press(KEY_KP_9); pin8_state = true; } 
  else if (!current_pin8_state && pin8_state) { Keyboard.release(KEY_KP_9); pin8_state = false; }

  pin2_state = current_pin2_state;
  pin3_state = current_pin3_state;
  pin4_state = current_pin4_state;
  pin5_state = current_pin5_state;
  pin6_state = current_pin6_state;
  pin7_state = current_pin7_state;
  pin8_state = current_pin8_state;
  pin9_state = current_pin9_state;
  pin10_state = current_pin10_state;
  pin16_state = current_pin16_state;
  pin21_state = current_pin21_state;

  delay(10);
}
