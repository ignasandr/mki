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

uint8_t getNoteChan();
uint8_t getSampChan();

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

void printVec(std::vector<uint8_t> &vect);

#endif