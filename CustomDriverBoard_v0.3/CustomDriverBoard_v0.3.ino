// v0.3
// 
// Sketch to connect a custom driver board
// as an emulated joystick.
// 
// by Kai Borah (Team 3620)
// 
// Joystick library by Matthew Heironimus 
// - https://github.com/MHeironimus/ArduinoJoystickLibrary
//-----------------------------------------------------------

#include <Joystick.h>

// Enable or disable button toggling.
int toggleMode = true;

//encoder variables
/*int val; 
int encoder0PinA = 11;
int encoder0PinB = 12;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;*/

void setup() {
  // Button pins
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);

  // Start reading joystick.
  Joystick.begin(true);
  delay(1000);
  
  // Starts with button #1 (button 0) set true.
  for (int j = 0; j < 5; j++) {
          Joystick.setButton(j, false);
        }
        Joystick.setButton(0, true);

  // Encoder pins
  /*pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  Serial.begin (9600);*/
}

// Constant that maps the phyical pin to the joystick button. 
// Put the first pin number here.
const int pinToButtonMap = 2;

// Put number of buttons and last state for each one here.
int lastButtonState[5] = {false,false,false,false,false};


void loop() {

  // Read button pin values.
  for (int index = 0; index < 5; index++) {
    int currentButtonState = !digitalRead(index + pinToButtonMap);
    if (currentButtonState != lastButtonState[index]) {
      // Physical button state changed.
      if (toggleMode) {
        if (currentButtonState) {
          // Physical button is depressed.
          for (int j = 0; j < 5; j++) {
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
  /*n = digitalRead(encoder0PinA);
   if ((encoder0PinALast == LOW) && (n == HIGH)) {
     if (digitalRead(encoder0PinB) == LOW) {
       encoder0Pos--;
     } else {
       encoder0Pos++;
     }
     Serial.print (encoder0Pos);
     Serial.print ("/");
   } 
   encoder0PinALast = n;*/

  delay(50);
}
