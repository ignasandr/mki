#include <input.h>

unsigned long previousBounceTime = 0;

const static uint8_t debounceTime = 25;
const static int8_t rising = HIGH - LOW;
const static int8_t falling = LOW - HIGH;

void init(uint8_t pin, uint8_t mode) {
    pinMode(pin, mode);
}

uint8_t stateChange(uint8_t pin, uint8_t &previousState) // read the button state check if the button has been pressed, debounce the button as well
{
  uint8_t stateChange = UNCHANGED;
  bool state = digitalRead(pin);               // read the button's state
  int8_t changeType = state - previousState;  // calculate the state change since last time

  if (millis() - previousBounceTime > debounceTime) { // check if the time since the last bounce is higher than the threshold
    if (changeType == falling) { // If the button is pressed (went from high to low)
        previousBounceTime = millis(); // remember when this happened
        stateChange = PRESSED; // the button is pressed
      }
    if (changeType == rising) { // if the button is released or bounces
        previousBounceTime = millis(); // remember when this happened
        stateChange = RELEASED; // the button is pressed
      }
  }

  previousState = state; // remember the current state
  return stateChange;
};