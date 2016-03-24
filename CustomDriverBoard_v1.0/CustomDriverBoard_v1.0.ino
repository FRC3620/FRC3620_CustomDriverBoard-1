//  Test version 0.6
//
//  Run our custom driver board
//  as an emulated joystick with buttons.
//
//  Arcade buttons are joystick 0-7
//  Encoder binaries are 8-10
//  
//  Pins in use:
//   -Encoder pins D0 and D1
//   -LCD I2C Pins D2 and D3 (SDA. SCL)
//   -Arcade buttons on pins D4-D11
//   
//
//  by Kai Borah (Team 3620)
//----------------------------------------------
//  TODO:
//  -Add global variable for # of buttons.
//  -Seperate auto and teleop LCD modes.
//  -Show auto modes in names, not numbers?

// Joystick Library
#include <Joystick.h>

// LCD Libraries
#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

// Encoder Library
#include <Encoder.h>

// Enable or disable button toggling.
int toggleMode = false;

Encoder myEnc(0, 1);

int encoderPush = !digitalRead(12);

//i2c LCD connection
Adafruit_LiquidCrystal lcd(0);

void setup() {

  // Button pins connected on 4-11
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

  // Starts with button 0 set true.
  for (int j = 0; j < 8; j++) {
    Joystick.setButton(j, false);
  }
  
  Joystick.setButton(0, true);

  Joystick.setZAxis(127);

  // Encoder knob push button
  pinMode(12, INPUT_PULLUP);
  Serial.begin(300);

  // LCD initiate.
  lcd.begin(16, 2);
  lcd.print(" Auto Mode:    *");
  lcd.setCursor(6, 1);
  lcd.print("Lane:       ");
  delay(500);

  Serial.println("setupBla");
}

// Constant that maps the phyical pin to the joystick button.
// Put the first pin number here.
const int pinToButtonMap = 4;

// Put number of buttons and last state for each one here.
int lastButtonState[4] = {false, false, false, false};

// For the encoder.
long oldPosition  = -999;

int encoderLast = false;
int encoderToggle = false;
int encoderToggleLast = false;
int whichAuto = 0;
int whichLane = 0;


void loop() {
  Serial.println("bla");

  // Read button pin values.
  for (int index = 0; index < 4; index++) {
    int currentButtonState = !digitalRead(index + pinToButtonMap);
    if (currentButtonState != lastButtonState[index]) {
      // Physical button state changed.
      if (toggleMode) {
        if (currentButtonState) {
          // Physical button is depressed.
          for (int j = 0; j < 4; j++) {
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

  int encoderCurrent = !digitalRead(12);
  int encoderJustPressed = false;

  // Read the encoder button.
  if (encoderCurrent != encoderLast) {
    // Encoder button state changed.
    if (encoderCurrent) {
      // Encoder button is depressed.
      encoderJustPressed = true;
    }
  }
  
//  Serial.println(!digitalRead(12));
 
//  Serial.print("Current is ");
//  Serial.println(encoderCurrent);
//  Serial.print("Last is ");
//  Serial.println(encoderLast);
//  Serial.print("Toggle is ");
//  Serial.println(encoderToggle);

   encoderLast = encoderCurrent;
   if (encoderJustPressed) {
    encoderToggle = !encoderToggle;
    if (encoderToggle) {
      // Picking lane
      myEnc.write(whichLane * 4);
      oldPosition = whichLane * 4;
      lcd.setCursor(15, 1);
      lcd.print("*");
      lcd.setCursor(15, 0);
      lcd.print(" ");
    }
    else {
      myEnc.write(whichAuto * 4);
      oldPosition = whichAuto * 4;
      lcd.setCursor(15, 0);
      lcd.print("*");
      lcd.setCursor(15, 1);
      lcd.print(" ");
    }
   }

   
  
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    Serial.print("Old position is ");
    Serial.println(oldPosition);
    Serial.print("New position is ");
    Serial.println(newPosition);
    oldPosition = newPosition;
    if (encoderToggle) {
      // Picking lane.
      whichLane = (newPosition / 4) % 6;
      if (whichLane < 0) {
        whichLane = whichLane + 6;
      }
      Serial.println("blabla");
      int bit1 = whichLane & 1;
      int bit2 = whichLane & 2;
      int bit4 = whichLane & 4;
      Joystick.setButton(5, bit1);
      Joystick.setButton(6, bit2);
      Joystick.setButton(7, bit4);
      lcd.setCursor(12, 1);
   // lcd.print("0   ");
      char d = '0' + whichLane;
      lcd.print(d);
    }
    else {
      // Picking auto.
      whichAuto = (newPosition / 4) % 8;
      if (whichAuto < 0) {
        whichAuto = whichAuto + 8;
      }
      Serial.println("elseblabla");
      int bit1 = whichAuto & 1;
      int bit2 = whichAuto & 2;
      int bit4 = whichAuto & 4;
      Joystick.setButton(8, bit1);
      Joystick.setButton(9, bit2);
      Joystick.setButton(10, bit4);
      lcd.setCursor(12, 0);
   // lcd.print("0   ");
      char c = '0' + whichAuto;
      lcd.print(c);
        
    }
  }
  Serial.println("preDelay");
  delay(50);
  Serial.println("postDelay");
}




