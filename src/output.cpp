#include <output.h>

void playFromArp() {
    if (getPlayheadPos() >= getArpSize()) resetPlayhead();  // reset playhead if position larger than arp vector size
    if (getMainMode() == DEF && getArpSize() > 0) { //if box in default mode and any note buttons are pressed
        uint8_t pin = getFromArp(getPlayheadPos());
        uint8_t note = getNoteByPin(pin);
        if (getCurrentlyPlaying() != 0) stopCurrentlyPlaying(); // check if a note is currently playing and stop it if that's the case
        defPlay(note, getNoteChan()); // play the note
    }
}

void defPlay(uint8_t note, uint8_t chan) {
    if(getCurrentCooldown(DEF) > 0) {
        play(note, calculateVelocity(DEF), chan);
        decrCurrentCooldown(DEF);
        updateDefCooldownThres();
        if(getCurrentCooldown(DEF) == 0 && getSeqBarsLeft() > 0) {
            setSeqBarsLeft(0);
            resetSeqPlayhead();
            incrCurrentCooldown(SEQ);
        }
        addToCurrentlyPlaying(note); // add
        incrPlayhead();
        timedStop(note, chan, getMappedRotaryValue());
    }
}

void sampPlay(uint8_t note, uint8_t chan) {
    if(getCurrentCooldown(SAMP) > 0) {
        play(note, 127, chan);
        play(note, 0, chan);
        decrCurrentCooldown(SAMP);
    }
}

// void holdPlay(uint8_t note, uint8_t chan) {
//     if(getCurrentCooldown(HOLD) > 800) {
//         if(getCurrentlyPlaying() > 0 && getCurrentlyPlaying() != note) {
//             timedStop(getCurrentlyPlaying(), getNoteChan(), 1);
//         }
//         play(note, calculateVelocity(HOLD), chan);
//         addToCurrentlyPlaying(note);
//         turnHoldAutoDecrOn();
//         // hold timer on
//     }
// }

void playFromSeq() {
    if(getSeqPlaying() || getSync()) {
        if(getSeqBarsLeft() > 0) {
            if(!getSeqPlaying()) setSeqPlaying(true);
            if(getSeqHit() > 0) {
                play(getCurrentSeqNote(), getSeqHit(), getSeqChan());
                play(getCurrentSeqNote(), 0, getSeqChan());
            }

            incrSeqPlayhead();

            if(getSeqPlayheadPos() >= 16) {
                resetSeqPlayhead();
                decrSeqBarsLeft();
            }
        }
        else {
            if(getSeqPlaying()) setSeqPlaying(false);
            if(getCurrentCooldown(SEQ) == 0) incrCurrentCooldown(SEQ);
        }
    }
}

void play(uint8_t note, uint8_t vel, uint8_t chan) {
    // Serial.print("MIDI.sendNoteOn(");
    // Serial.print(note);
    // Serial.print(", ");
    // Serial.print(vel);
    // Serial.print(", ");
    // Serial.print(chan);
    // Serial.println(");");

    int command = chan + 143;
    Serial.write(command);
    Serial.write(note);
    Serial.write(vel);
}

void timedStop(uint8_t note, uint8_t chan, uint8_t numberOfTicks) {
    turnStopCounterOn(note, chan, numberOfTicks); // also set getNoteToStop() and getNoteToStopChan()
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