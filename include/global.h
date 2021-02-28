#include <Arduino.h> 
#include <StandardCplusplus.h>
#include <algorithm>
#include <vector>
#ifndef GLOBAL_H
#define GLOBAL_H

enum{UNCHANGED, PRESSED, RELEASED};
enum{DEF, SAMP, HOLD};

struct button {
  uint8_t pin;
  uint8_t state;
  unsigned long bounceTime;
};

struct note {
  uint8_t pin;
  uint8_t note;
  uint8_t snote;
};

uint8_t getMainMode();
void setMainMode(uint8_t mode);

boolean getIntClock();
void intClockOn();
void intClockOff();


void addToArp(uint8_t pin);
void removeFromArp(uint8_t pin);
uint8_t getFromArp(uint8_t id);
uint8_t getArpSize();

uint8_t getPlayheadPos();
void incrPlayhead();
void resetPlayhead();

uint8_t getCurrentlyPlaying();
void addToCurrentlyPlaying(uint8_t note);
void clearCurrentlyPlaying();

//utils
void printVec(std::vector<uint8_t> &vect);
uint8_t getNoteByPin(uint8_t pin);
uint8_t getSnoteByPin(uint8_t pin);
uint16_t getRotaryValue();
uint8_t getMappedRotaryValue();
uint8_t getNoteChan();
uint8_t getSampChan();

uint8_t getDivisionTicks();
void setDivisionTicks(uint8_t numberOfTicks);

boolean getStopCounterOn();
void turnStopCounterOn(uint8_t note, uint8_t chan, uint8_t numberOfTicks);
void turnStopCounterOff();
uint8_t getNoteToStop();
uint8_t getNoteToStopChan();

uint8_t getStopCounter();
void decrStopCounter();
void setStopCounter(uint8_t numberOfTicks);


uint16_t getCurrentCooldown(uint8_t mode);
void incrCurrentCooldown(uint8_t mode);
void decrCurrentCooldown(uint8_t mode);

boolean getHoldAutoDecrOn();
void turnHoldAutoDecrOn();
void turnHoldAutoDecrOff();

#endif