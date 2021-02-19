#include <output.h>

void playFromArp() {
    if (getMainMode() == DEF && getArpSize() > 0) { //if box in default mode and any note buttons are pressed
        if (getPlayheadPos() > getArpSize()) resetPlayhead();  // reset playhead if position larger than arp vector size
        uint8_t note = getFromArp(getPlayheadPos());

        if (getCurrentlyPlaying() != 0) stop(); // check if a note is currently playing and stop it if that's the case
        play(note, 127, getNoteChan()); // play the note
        addToCurrentlyPlaying(note); // add
        incrPlayhead();
        timedStop();

    }
}

void timedStop() {
    Serial.println("Stopping in a bit");
}

void play(uint8_t note, uint8_t vel, uint8_t chan) {
    Serial.print("MIDI.sendNoteOn(");
    Serial.print(note);
    Serial.print(vel);
    Serial.print(chan);
    Serial.println(");");
}

void stop() {
    play(getCurrentlyPlaying(), 0, getNoteChan());
    clearCurrentlyPlaying();
}