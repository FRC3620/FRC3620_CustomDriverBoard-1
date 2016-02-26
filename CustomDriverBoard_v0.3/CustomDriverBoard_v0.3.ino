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

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);

  Joystick.begin(true);

  delay(1000);
  
  // Starts with button #1 (button 0) set true.
  for (int j = 0; j < 5; j++) {
          Joystick.setButton(j, false);
        }
        Joystick.setButton(0, true);
}

// Constant that maps the phyical pin to the joystick button. 
// Put the first pin number here.
const int pinToButtonMap = 2;

// Put number of buttons and last state for each one here.
int lastButtonState[5] = {false,false,false,false,false};


void loop() {

  // Read pin values
  for (int index = 0; index < 5; index++)
  {
    int currentButtonState = !digitalRead(index + pinToButtonMap);
    if (currentButtonState != lastButtonState[index])
    {
      // Buttons toggle. For no-toggle, replace w/ 
      // Joystick.setButton(index, currentButtonState);
      // Physical button state changed.
      if (currentButtonState) {
        // Physical button is depressed.
        for (int j = 0; j < 5; j++) {
          Joystick.setButton(j, false);
        }
        Joystick.setButton(index, true);
      }
      lastButtonState[index] = currentButtonState;
    }
  }

  delay(50);
}
