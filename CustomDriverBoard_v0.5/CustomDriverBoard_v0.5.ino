// v0.5
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
#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

// Encoder Library
#include <Encoder.h>

// Enable or disable button toggling.
int toggleMode = true;

Encoder myEnc(0, 1);

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
  
  // LCD initiate.
  lcd.begin(16, 2);
  // Put LCD message here
  lcd.print(" Auto Mode: NONE");
  lcd.setCursor(0, 1);
  //lcd.print("Angle Mode: A");
}

// Constant that maps the phyical pin to the joystick button. 
// Put the first pin number here.
const int pinToButtonMap = 4;

// Put number of buttons and last state for each one here.
int lastButtonState[8] = {false,false,false,false,false,false,false,false};

// For the encoder.
long oldPosition  = -999;


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
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    long which = (newPosition / 4) % 8;
    if (which < 0) {
      which = which + 8;
    }
    Serial.println(which);

  //Run LCD loop and encoder to button output.
  for (int index = 0; index < 5; index++) {
    if (which == 0) {
      Joystick.setButton(10, false);
      Joystick.setButton(11, false);
      Joystick.setButton(12, false);
      lcd.setCursor(12, 0);
      lcd.print("1   ");
    }
    if (which == 1) {
      Joystick.setButton(10, true);
      Joystick.setButton(11, false);
      Joystick.setButton(12, false);
      lcd.setCursor(12, 0);
      lcd.print("2   ");
    }
    if (which == 2) {
      Joystick.setButton(10, false);
      Joystick.setButton(11, true);
      Joystick.setButton(12, false);
      lcd.setCursor(12, 0);
      lcd.print("3   ");
    }
    if (which == 3) {
      Joystick.setButton(10, true);
      Joystick.setButton(11, true);
      Joystick.setButton(12, false);
      lcd.setCursor(12, 0);
      lcd.print("4   ");
    }
    if (which == 4) {
      Joystick.setButton(10, false);
      Joystick.setButton(11, false);
      Joystick.setButton(12, true);
      lcd.setCursor(12, 0);
      lcd.print("5   ");
    }
    if (which == 5) {
      Joystick.setButton(10, true);
      Joystick.setButton(11, false);
      Joystick.setButton(12, true);
      lcd.setCursor(12, 0);
      lcd.print("6   ");
    }
    if (which == 6) {
      Joystick.setButton(10, false);
      Joystick.setButton(11, true);
      Joystick.setButton(12, true);
      lcd.setCursor(12, 0);
      lcd.print("7   ");
    }
    if (which == 7) {
      Joystick.setButton(10, true);
      Joystick.setButton(11, true);
      Joystick.setButton(12, true);
      lcd.setCursor(12, 0);
      lcd.print("8   ");
    }
    /*if (!digitalRead(7)) {
      lcd.setCursor(12, 1);
      lcd.print("A   ");
    }
    if (!digitalRead(8)) {
      lcd.setCursor(12, 1);
      lcd.print("B   ");
    }*/
  }
  delay(50); 
  }
}



  
