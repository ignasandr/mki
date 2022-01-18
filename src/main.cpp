#include <Arduino.h>
#include <global.h>
#include <input.h>
#include <routers.h>
#include <ArduinoSTL.h>
#include <vector>

using namespace std;

boolean byteReady = false;
byte midiByte = 0;

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
  Serial.begin(31250); // MIDI Serial
  // Serial.begin(9600);
  // Serial.println("Serial working");

  for (auto arr : initPinModes) {
    init(arr[0], arr[1]);
    if (arr[1] == INPUT_PULLUP)
      {
        button newButton = {arr[0], HIGH, 0};
        buttonState.push_back(newButton);
      }
  }
  if(digitalRead(9) == LOW) {
    setDivisionTicks(6); // if Switch up, set arp speed to 16th
  }
}


void loop() {
  for (auto &button : buttonState) {
    uint8_t current = stateChange(button.pin, button.state, button.bounceTime);
    if (current > 0) {
      mainRouter(button.pin, current);
    }
  }
  if(Serial.available() > 0) {
    midiByte = Serial.read();
    if(midiByte == 0xF8) handleTicks();
    byteReady = true;
  }
  if (byteReady) {
    byteReady = false;
    Serial.write(midiByte);
  }
  // generateTicks();
  cooldownClock();
  ledRouter();
}