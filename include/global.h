#include <Arduino.h> 
#include <ArduinoSTL.h>
#include <algorithm>
#include <vector>
#ifndef GLOBAL_H
#define GLOBAL_H

enum{UNCHANGED, PRESSED, RELEASED};
enum{DEF, SAMP, SEQ};

struct button {
  uint8_t pin;
  uint8_t state;
  unsigned long bounceTime;
};

struct note {
  uint8_t pin;
  uint8_t note;
  uint8_t sampnote;
  uint8_t seqnote;
  uint8_t seqlength;
  uint8_t seqnumber;
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

// arp playhead

uint8_t getPlayheadPos();
void incrPlayhead();
void resetPlayhead();

// seq playhead

uint8_t getSeqPlayheadPos();
void incrSeqPlayhead();
void resetSeqPlayhead();

uint8_t getCurrentlyPlaying();
void addToCurrentlyPlaying(uint8_t note);
void clearCurrentlyPlaying();

//utils
void printVec(std::vector<uint8_t> &vect);
uint8_t getNoteByPin(uint8_t pin);
uint8_t getSampNoteByPin(uint8_t pin);
uint8_t getSeqNoteByPin(uint8_t pin);
uint8_t getSeqLengthByPin(uint8_t pin);
uint8_t getSeqNumberByPin(uint8_t pin);
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


boolean getCooldownClock(uint8_t mode);
void setCooldownClock(uint8_t mode, boolean value);
uint16_t getCooldownTime(uint8_t mode);
void incrCooldownTime(uint8_t mode);
void resetCooldownTime(uint8_t mode);
uint16_t getCooldownThreshold(uint8_t mode);
void setCooldownThreshold(uint8_t mode, unsigned long value);
void updateDefCooldownThres();

uint16_t getCurrentCooldown(uint8_t mode);
void incrCurrentCooldown(uint8_t mode);
void decrCurrentCooldown(uint8_t mode);
void setCurrentCooldown(uint8_t mode, uint8_t value);
uint16_t getMaxCooldown(uint8_t mode);

// boolean getHoldAutoDecrOn();
// void turnHoldAutoDecrOn();
// void turnHoldAutoDecrOff();


uint8_t getSeqBarsLeft();
void setSeqBarsLeft(uint8_t newBars);
void decrSeqBarsLeft();
uint8_t getCurrentSeqNote();
void setCurrentSeqNote(uint8_t newNote);
uint8_t getCurrentSeqNumber();
void setCurrentSeqNumber(uint8_t newNumber);
uint8_t getSeqChan();
uint8_t getSeqHit();

uint8_t calculateVelocity(uint8_t mode);

#endif