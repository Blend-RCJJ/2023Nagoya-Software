#include "mlt8530.h"

void MLT8530::setFrequncy(int freq) {
    analogWriteFrequency(freq);
    pin = 0.5;
}

void MLT8530::mute(void) {
    pin = 0;
}

void MLT8530::bootSound(void) {
    setFrequncy(440);
    delay(100);
    setFrequncy(880);
    delay(100);
    mute();
}