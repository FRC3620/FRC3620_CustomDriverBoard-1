// v0.4
// TODO Add global variable for # of buttons.
// 
// Sketch to connect a custom driver board
// as an emulated joystick.
// 
// by Kai Borah (Team 3620)
// 
// Joystick library by Matthew Heironimus 
// - https://github.com/MHeironimus/ArduinoJoystickLibrary
//-----------------------------------------------------------

// Joystick Library
#include <Joystick.h>

// LCD Libraries
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

// Enable or disable button toggling.
int toggleMode = true;

// Encoder variables
int val; 
int encoder0PinA = A0;
int encoder0PinB = A1;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;

//i2c LCD connection
Adafruit_LiquidCrystal lcd(0);

void setup() {
  // Button pins connected on 4-8
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);

  // Start reading joystick.
  Joystick.begin(true);
  delay(1000);
  
  // Starts with button #1 (button 0) set true.
  for (int j = 0; j < 8; j++) {
          Joystick.setButton(j, false);
        }
        Joystick.setButton(0, true);

  // Encoder pins
  pinMode (encoder0PinA, INPUT_PULLUP);
  pinMode (encoder0PinB, INPUT_PULLUP);
  Serial.begin (115200);

  lcd.begin(16, 2);
  // Put LCD message here
  lcd.print(" Auto Mode: NONE");
  lcd.setCursor(0, 1);
  lcd.print("Angle Mode: A");
}

// Constant that maps the phyical pin to the joystick button. 
// Put the first pin number here.
const int pinToButtonMap = 4;

// Put number of buttons and last state for each one here.
int lastButtonState[8] = {false,false,false,false,false,false,false,false};


void loop() {

  // Read button pin values.
  for (int index = 0; index < 8; index++) {
    int currentButtonState = !digitalRead(index + pinToButtonMap);
    if (currentButtonState != lastButtonState[index]) {
      // Physical button state changed.
      if (toggleMode) {
        if (currentButtonState) {
          // Physical button is depressed.
          for (int j = 0; j < 8; j++) {
            Joystick.setButton(j, false);
          }
          Joystick.setButton(index, true);
        }
      }
      else {
        Joystick.setButton(index, currentButtonState);
      }
      lastButtonState[index] = currentButtonState;
    }
  }
  
  // Read the encoder.
  n = digitalRead(encoder0PinA);
   if ((encoder0PinALast == LOW) && (n == HIGH)) {
     if (digitalRead(encoder0PinB) == LOW) {
       encoder0Pos--;
     } else {
       encoder0Pos++;
     }
     Serial.print (encoder0Pos);
     Serial.print ("/");
   } 
   encoder0PinALast = n;

   //Run LCD loop
  /*lcd.setCursor(4, 1);
  lcd.print("(-^_^ )-");
  delay(500);
  lcd.setCursor(4, 1);
  lcd.print("        ");
  lcd.setCursor(3, 1);
  lcd.print("-( ^_^-)");
  delay(500);
  lcd.setCursor(3, 1);
  lcd.print("       ");*/
  for (int index = 0; index < 5; index++) {
    if (!digitalRead(4)) {
      lcd.setCursor(12, 0);
      lcd.print("1   ");
    }
    if (!digitalRead(5)) {
      lcd.setCursor(12, 0);
      lcd.print("2   ");
    }
    if (!digitalRead(6)) {
      lcd.setCursor(12, 0);
      lcd.print("NONE");
    }
    if (!digitalRead(7)) {
      lcd.setCursor(12, 1);
      lcd.print("A   ");
    }
    if (!digitalRead(8)) {
      lcd.setCursor(12, 1);
      lcd.print("B   ");
    }
  }

  delay(50);
}
