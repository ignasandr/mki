#include <output.h>

void playFromArp() {
    if (getPlayheadPos() >= getArpSize()) resetPlayhead();  // reset playhead if position larger than arp vector size
    if (getMainMode() == DEF && getArpSize() > 0) { //if box in default mode and any note buttons are pressed
        uint8_t pin = getFromArp(getPlayheadPos());
        uint8_t note = getNoteByPin(pin);
        if (getCurrentlyPlaying() != 0) stop(); // check if a note is currently playing and stop it if that's the case
        play(note, 127, getNoteChan()); // play the note
        addToCurrentlyPlaying(note); // add
        incrPlayhead();
        timedStop();
    }
}

void timedStop() {
    // Serial.println("Stopping in a bit");
    stop();
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

void stop() {
    play(getCurrentlyPlaying(), 0, getNoteChan());
    clearCurrentlyPlaying();
}