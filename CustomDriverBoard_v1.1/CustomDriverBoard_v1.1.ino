//  Final v1.1
//
//  Run the custom control deck as an emulated joystick with buttons.
//
//  Arcade buttons are joystick 0-4
//  Lane binaries are joystick 5-7
//  Auto binaries are joystick 8-10
//  
//  
//  Pins in use:
//   -Encoder pins D0 and D1
//   -LCD I2C Pins D2 and D3 (SDA. SCL)
//   -Arcade buttons on pins D4-D7
//   
//
//  by Kai Borah (Team 3620)
//  --------------------------------------------------------------
//  TODO:
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

// Constant that maps the phyical pin to the joystick button.
// Put the first pin number here.
const int pinToButtonMap = 4;

// Put number of buttons and last state for each one here.
int lastButtonState[4] = {false, false, false, false};

// For the encoder.
long oldPosition  = -999;

int encoderLast = false;

#define MODE_AUTO 0
#define MODE_LANE 1
#define MODE_NONE 2
#define MODE_MAX 2

int encoderMode = MODE_NONE;

int whichAuto = 0;
int whichLane = 0;

#define N_AUTO 10
String autoLabels[N_AUTO] = { "None   ", "CDF     ", "CDF2    ", "LoBar   ", "Moat    ", "Port    ", "RoughTer", "ReachDef", "LB&Shoot", "LB&HiSho"};
#define N_LANES 7
String laneLabels[N_LANES] = { "No", "2<", "2^", "3^", "4^", "5^", "5>" };

void updateAuto() {
  lcd.setCursor(7, 0);
  lcd.print(autoLabels[whichAuto]);
}

void updateLane() {
  lcd.setCursor(7, 1);
  // lcd.print("0   ");
  // char d = '0' + whichLane;
  lcd.print(laneLabels[whichLane]);
}

void updateModeIndicators() {
  if (encoderMode == MODE_AUTO) {
    myEnc.write(whichAuto * 4 + 2);
    oldPosition = whichAuto * 4 + 2;
    lcd.setCursor(0, 0);
    lcd.print("*");
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(15, 0);
    lcd.print("*");
    lcd.setCursor(15, 1);
    lcd.print(" ");
  } else if (encoderMode == MODE_LANE) {
    // Picking lane
    myEnc.write(whichLane * 4 + 2);
    oldPosition = whichLane * 4 + 2;
    lcd.setCursor(0, 0);
    lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print("*");
    lcd.setCursor(15, 0);
    lcd.print(" ");
    lcd.setCursor(15, 1);
    lcd.print("*");
  } else {
    lcd.setCursor(0, 0);
    lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(15, 0);
    lcd.print(" ");
    lcd.setCursor(15, 1);
    lcd.print(" ");
  }
}

void setup() {

  // Button pins connected on 4-11
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
//  pinMode(8, INPUT_PULLUP);
//  pinMode(9, INPUT_PULLUP);
//  pinMode(10, INPUT_PULLUP);
//  pinMode(11, INPUT_PULLUP);

  // Start reading joystick.
  Joystick.begin(true);
  delay(1000);

  // Starts with button 0 set true.
  for (int j = 0; j < 8; j++) {
    Joystick.setButton(j, false);
  }
  
  Joystick.setButton(0, true);

  // wiggle the joystick so that the robot gets an update
  Joystick.setZAxis(0);
  delay(100);
  Joystick.setZAxis(127);

  // Encoder knob push button
  pinMode(12, INPUT_PULLUP);
  Serial.begin(300);

  // LCD initiate.
  lcd.begin(16, 2);
  lcd.print(" Auto:");
  lcd.setCursor(1, 1);
  lcd.print("Lane:");

  updateAuto();
  updateLane();
  updateModeIndicators();
  
  delay(500);
}

void loop() {
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
 
//  Serial.print("Current is ");
//  Serial.println(encoderCurrent);
//  Serial.print("Last is ");
//  Serial.println(encoderLast);
//  Serial.print("Toggle is ");
//  Serial.println(encoderToggle);

  encoderLast = encoderCurrent;
  if (encoderJustPressed) {
    encoderMode = encoderMode + 1;
    if (encoderMode > MODE_MAX) encoderMode = 0;
    updateModeIndicators();
  }
  
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    Serial.print("Old position is ");
    Serial.println(oldPosition);
    Serial.print("New position is ");
    Serial.println(newPosition);
    oldPosition = newPosition;
    if (encoderMode == MODE_AUTO) {
      // Picking auto.
      whichAuto = (newPosition / 4) % N_AUTO;
      if (whichAuto < 0) {
        whichAuto = whichAuto + N_AUTO;
      }
      int bit1 = whichAuto & 1;
      int bit2 = whichAuto & 2;
      int bit4 = whichAuto & 4;
      int bit8 = whichAuto & 8;
      Joystick.setButton(7, bit1);
      Joystick.setButton(8, bit2);
      Joystick.setButton(9, bit4);
      Joystick.setButton(10, bit8);
      updateAuto();
    } else if (encoderMode == MODE_LANE) {
      // Picking lane.
      whichLane = (newPosition / 4) % N_LANES;
      if (whichLane < 0) {
        whichLane = whichLane + N_LANES;
      }
      int bit1 = whichLane & 1;
      int bit2 = whichLane & 2;
      int bit4 = whichLane & 4;
      Joystick.setButton(4, bit1);
      Joystick.setButton(5, bit2);
      Joystick.setButton(6, bit4);
      updateLane();
    }
  }
  delay(50);
}

