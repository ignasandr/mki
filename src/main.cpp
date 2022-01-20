#include <Arduino.h>
#include <global.h>
#include <input.h>
#include <routers.h>
#include <ArduinoSTL.h>
#include <vector>

using namespace std;

#define MIDI_START 0xFA
#define MIDI_STOP 0xFC
#define MIDI_CLOCK 0xF8

byte midiByte;

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

  // MIDI Thru
  if(Serial.available() > 0) { 
    midiByte = Serial.read();

    if(midiByte == MIDI_START) {
      setExtClock(true);
      resetSync();
    }

    if(midiByte == MIDI_STOP) {
      setExtClock(false);
    }

    if(extClockOn() && midiByte == MIDI_CLOCK) {
      handleTicks();
    }
  }
  // generateTicks();
  cooldownClock();
  ledRouter();
}