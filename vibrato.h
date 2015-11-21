// Copyright (c) 2015 Peter Teichman

#ifndef VIBRATO_H
#define VIBRATO_H

#include <Audio.h>

// Simulate the Hammond Chorus/Vibrato scanner. Originally, this is a
// 1ms delay line with 9 taps. The circuit crossfades between the taps
// with a triangle wave from 1 to 9 and then back. This modulation wave
// has a frequency of 7Hz.

// A Teensy audio block is 128 samples at 44.1kHz, so roughly 2.9ms.
// We'll keep one block around to maintain a 1ms ring buffer. Each
// update() cycle reads the input block, writes it to the ring buffer,
// and writes phase modulated output.

class Vibrato : public AudioStream {
 public:
    Vibrato() : AudioStream(1, inputQueueArray) {
    }

    void Init() {
        // The ring buffer write pointer is initialized to give a
        // delay of 1ms.
        wp = 34; // 44117.64706Hz * 0.001s / 128 samples = 34 samples
        for (int i=0; i<AUDIO_BLOCK_SAMPLES; i++) {
            buf[i] = 0;
        }

        scan_phase = 0;
    }

    void update(void);

 private:
    audio_block_t *inputQueueArray[1];

    // Ring buffer. The write pointer is stored here; the read pointer
    // is calculated at read time from wp.
    int wp;
    int16_t buf[AUDIO_BLOCK_SAMPLES];

    // Scanner position and scan direction. This modulates the read
    // pointer into buf as a 7Hz triangle wave. We'll use 15 bits for
    // position and the 16th to detect overflow.
    //
    // The write pointer is 34 samples ahead, so use 6 bits (5 + sign)
    // as the position modifier so reads never outrun writes.
    uint32_t scan_phase;
};

#endif
