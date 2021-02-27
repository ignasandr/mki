#include <input.h>

const static uint8_t debounceTime = 25;
const static int8_t rising = HIGH - LOW;
const static int8_t falling = LOW - HIGH;

void init(uint8_t pin, uint8_t mode) {
    pinMode(pin, mode);
}

uint8_t stateChange(uint8_t pin, uint8_t &previousState, unsigned long &previousBounceTime) // read the button state check if the button has been pressed, debounce the button as well
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

uint8_t beats_per_minute = 120;
uint16_t milliseconds_per_minute = 60000;
float MS_per_beat = milliseconds_per_minute/beats_per_minute;
unsigned long last_time = 0;

void generateTicks() {
  unsigned long current_time = millis();    //Time reference. Elapsed time since board is on
  float ticks = MS_per_beat/24;      //96 ticks per beat

   uint8_t elapsed_time = current_time - last_time;
    
    if(elapsed_time >= ticks && getIntClock() == true)
    {
      last_time = current_time;
      handleTicks();
    }
}

void cooldownClock() {
  static const uint16_t INTERVAL = 1; // ms
	static unsigned long lastRefreshTime = 0; 

  toggleCooldown(); // ::cooldown.cpp

  uint8_t modes[] = {DEF, SAMP, HOLD};
  if (millis() - lastRefreshTime > INTERVAL) {
    for (auto mode: modes) {
      if(cooldownClockOn(mode) == true) 
        replenish(mode);
    }
    lastRefreshTime = millis();
  }
}