#include <output.h>

void playFromArp() {
    if (getPlayheadPos() >= getArpSize()) resetPlayhead();  // reset playhead if position larger than arp vector size
    if (getMainMode() == DEF && getArpSize() > 0) { //if box in default mode and any note buttons are pressed
        uint8_t pin = getFromArp(getPlayheadPos());
        uint8_t note = getNoteByPin(pin);
        if (getCurrentlyPlaying() != 0) stopCurrentlyPlaying(); // check if a note is currently playing and stop it if that's the case
        defPlay(note, 127, getNoteChan()); // play the note
    }
}

void defPlay(uint8_t note, uint8_t vel, uint8_t chan) {
    if(getCurrentCooldown(DEF) > 0) {
        play(note, vel, chan);
        decrCurrentCooldown(DEF);
        addToCurrentlyPlaying(note); // add
        incrPlayhead();
        timedStop(note, chan, getMappedRotaryValue());
    }
}

void sampPlay(uint8_t note, uint8_t vel, uint8_t chan) {
    if(getCurrentCooldown(SAMP) > 0) {
        play(note, vel, chan);
        decrCurrentCooldown(SAMP);
    }
}

void holdPlay(uint8_t note, uint8_t vel, uint8_t chan) {
    if(getCurrentCooldown(HOLD) > 0) {
        if(getCurrentlyPlaying() > 0 && getCurrentlyPlaying() != note) {
            timedStop(getCurrentlyPlaying(), getNoteChan(), 1);
        }
        play(note, vel, chan);
        addToCurrentlyPlaying(note);
        // hold timer on
    }
}

void play(uint8_t note, uint8_t vel, uint8_t chan) {
    Serial.print("MIDI.sendNoteOn(");
    Serial.print(note);
    Serial.print(", ");
    Serial.print(vel);
    Serial.print(", ");
    Serial.print(chan);
    Serial.println(");");
}

void timedStop(uint8_t note, uint8_t chan, uint8_t numberOfTicks) {
    turnStopCounterOn(note, chan, numberOfTicks);
}

void stopOrDecr() {
    if (getStopCounter() == 0) {
        stop(getNoteToStop(), getNoteToStopChan());
        if(getCurrentlyPlaying() == getNoteToStop()) clearCurrentlyPlaying();
        turnStopCounterOff();
    } else {
        decrStopCounter();
    }
}

void stopCurrentlyPlaying() {
    play(getCurrentlyPlaying(), 0, getNoteChan());
    clearCurrentlyPlaying();
}

void stop(uint8_t note, uint8_t chan) {
    play(note, 0, chan);
}