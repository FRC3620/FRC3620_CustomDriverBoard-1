// v0.1
// 
// Sketch to connect a custom driver board
// as an emulated joystick.
// 
// by Kai Borah (Team 3620)
// 
// Joystick library by Matthew Heironimus
//---------------------------------------------

#include <Joystick.h>

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);

  Joystick.begin();
}

// Constant that maps the phyical pin to the joystick button. 
// Put the first pin number here.
const int pinToButtonMap = 2;

// Put number of buttons and last state for each one here.
int lastButtonState[5] = {0,0,0,0,0};

void loop() {

  // Read pin values
  for (int index = 0; index < 5; index++)
  {
    int currentButtonState = !digitalRead(index + pinToButtonMap);
    if (currentButtonState != lastButtonState[index])
    {
      Joystick.setButton(index, currentButtonState);
      lastButtonState[index] = currentButtonState;
    }
  }

  delay(50);
}
