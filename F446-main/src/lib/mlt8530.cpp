#include "mlt8530.h"

void MLT8530::setFrequncy(int freq) {
    analogWriteFrequency(freq);
    pin = 0.5;
}

void MLT8530::mute(void) {
    pin = 0;
}