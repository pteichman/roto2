// Copyright (c) 2015 Peter Teichman

#include "drawbar_osc.h"

int16_t DrawbarWave[DRAWBAR_WAVETABLE_LENGTH+1];
int32_t Buf[DRAWBAR_WAVETABLE_LENGTH+1];

void DrawbarOsc::update(void) {
    audio_block_t *block;
    int16_t *bp;
    uint32_t index, scale;
    int32_t v1, v2, v3;

    if (amp == 0) {
        return;
    }

    block = allocate();

    if (block) {
        bp = block->data;
        for (int i=0; i<AUDIO_BLOCK_SAMPLES; i++) {
            // Use the top 10 bits as index into the wavetable.
            index = tone_phase >> 22;
            scale = (tone_phase >> 6) & 0xFFFF;

            // Linear interpolation between v1 and v2.
            v1 = DrawbarWave[index] * (0xFFFF - scale);
            v2 = DrawbarWave[index+1] * scale;
            v3 = (v1 + v2) >> 16;

            *bp++ = (int16_t)((v3 * amp) >> 15);

            tone_phase += tone_incr;
            if (tone_phase >= 0xc0000000) {
                // Wrap tone_phase once its high bits are 0xc. This makes
                // the top 10 bits wrap once they hit 768, the length of our
                // wavetable.
                tone_phase -= 0xc0000000;
            }
        }
    }

    transmit(block, 0);
    release(block);
}

void DrawbarOsc::Rebuild(void) {
    // Accumulating 24 bits per drawbar (16 sine * 8 volume), then the
    // 9 drawbars accumulate together for 4 more bits. So shift 12 at
    // the end to get back to 16 bits.

    // Most of the Hammond partial frequencies are integer multiples
    // of the fundamental, so we can index the desired values
    // directly. The first two drawbars (0.5f and 1.5f), on the other
    // hand, need interpolation.
    //
    // Use a 32-bit phase index and increment. The high 8 bits of
    // index will be our sine index; the next 16 will be the scale for
    // interpolation.
    uint32_t index=0, scale=0;
    uint32_t phase=0;
    int32_t v1, v2, v3;
    for (int i=0; i<DRAWBAR_WAVETABLE_LENGTH; i++) {
        index = phase >> 24;
        scale = (phase >> 8) & 0xFFFF;

        // Linear interpolation between v1 and v2.
        v1 = AudioWaveformSine[index] * (0xFFFF - scale);
        v2 = AudioWaveformSine[index+1] * scale;
        v3 = (v1 + v2) >> 16;

        Buf[i] = v3 * drawbars[0];

        // Incrementing with 0x1000000 will get us the fundamental
        // frequency in the high 8 bits of phase; there are three
        // fundamental cycles per one of these, so use 0x1000000/3.
        phase += 0x555555;
    }

    for (int i=0; i<DRAWBAR_WAVETABLE_LENGTH; i++) {
        index = phase >> 24;
        scale = (phase >> 8) & 0xFFFF;

        // Linear interpolation between v1 and v2.
        v1 = AudioWaveformSine[index] * (0xFFFF - scale);
        v2 = AudioWaveformSine[index+1] * scale;
        v3 = (v1 + v2) >> 16;

        Buf[i] += v3 * drawbars[1];

        phase += 0x1555555;
    }

    // Three cycles of the fundamental frequency.
    for (int i=0; i<DRAWBAR_WAVETABLE_LENGTH; i++) {
        Buf[i] += AudioWaveformSine[i & 0xff] * drawbars[2];
    }

    for (int i=0; i<DRAWBAR_WAVETABLE_LENGTH; i++) {
        Buf[i] += AudioWaveformSine[2*i & 0xff] * drawbars[3];
    }

    for (int i=0; i<DRAWBAR_WAVETABLE_LENGTH; i++) {
        Buf[i] += AudioWaveformSine[3*i & 0xff] * drawbars[4];
    }

    for (int i=0; i<DRAWBAR_WAVETABLE_LENGTH; i++) {
        Buf[i] += AudioWaveformSine[4*i & 0xff] * drawbars[5];
    }

    for (int i=0; i<DRAWBAR_WAVETABLE_LENGTH; i++) {
        Buf[i] += AudioWaveformSine[5*i & 0xff] * drawbars[6];
    }

    for (int i=0; i<DRAWBAR_WAVETABLE_LENGTH; i++) {
        Buf[i] += AudioWaveformSine[6*i & 0xff] * drawbars[7];
    }

    for (int i=0; i<DRAWBAR_WAVETABLE_LENGTH; i++) {
        Buf[i] += AudioWaveformSine[8*i & 0xff] * drawbars[8];
    }

    for (int i=0; i<DRAWBAR_WAVETABLE_LENGTH; i++) {
        DrawbarWave[i] = Buf[i] >> 12;
    }
    DrawbarWave[DRAWBAR_WAVETABLE_LENGTH] = DrawbarWave[0];
}
