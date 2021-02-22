#include <Arduino.h>
#include <global.h>
#include <input.h>
#include <routers.h>
#include <StandardCplusplus.h>
#include <vector>

using namespace std;

const uint8_t initPinModes[][2] =
{
    { 2, INPUT_PULLUP}, // notes
    { 3, INPUT_PULLUP},
    { 4, INPUT_PULLUP},
    { 5, INPUT_PULLUP},
    { 6, INPUT_PULLUP},
    { 7, INPUT_PULLUP},
    { 8, INPUT_PULLUP},
    { 9, INPUT_PULLUP}, // switch
    { 12, INPUT_PULLUP}, // shift
    { 13, INPUT_PULLUP},
    { 10, OUTPUT}, // leds
    { 11, OUTPUT}
};

vector<struct button> buttonState;

void setup() {
  Serial.begin(57600);
  Serial.println("Serial working");

  for (auto arr : initPinModes) {
    init(arr[0], arr[1]);
    if (arr[1] == INPUT_PULLUP)
      {
        button newButton = {arr[0], HIGH, 0};
        buttonState.push_back(newButton);
      }
  }
}


void loop() {
  for (auto &button : buttonState) {
    uint8_t current = stateChange(button.pin, button.state, button.bounceTime);
    if (current > 0) {
      mainRouter(button.pin, current);
    }
  }
  generateTicks();
  cooldownClock();
}