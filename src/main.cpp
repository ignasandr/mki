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
#define MIDI_NOTE_ON 0x90

boolean byteReady = false;
byte midiByte = 0;
byte midiCmd = 0;
byte midiState = 0;
byte sendCmd = 0;
byte sendNote = 0;
byte sendVel = 0;


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
    midiCmd = midiByte & B11110000;

    if(midiByte == MIDI_START) {
      setExtClock(true);
      resetSync();
      Serial.write(midiByte);
    }

    if(midiByte == MIDI_STOP) {
      setExtClock(false);
      Serial.write(midiByte);
    }

    if(extClockOn() && midiByte == MIDI_CLOCK) {
      handleTicks();
      Serial.write(midiByte);
    }
    switch(midiState) {
      case 0:
        if (midiCmd == 0x90) {
          sendCmd = midiByte;
          midiState = 1;
        }
        break;
      case 1:
        if (midiByte < 128) {
          sendNote = midiByte;
          midiState = 2;
        }
        else {
          midiState = 0;
        }
        break;
      case 2:
        if (midiByte < 128) {
          sendVel = midiByte;
          Serial.write(sendCmd);
          Serial.write(sendNote);
          Serial.write(sendVel);
          midiState = 0;
        }
        else {
          midiState = 0;
        }
        break;
    }
      
  }

  // generateTicks();
  cooldownClock();
  ledRouter();
}