// Copyright (c) 2015 Peter Teichman

#include "drawbar_osc.h"

int16_t DrawbarWave[DRAWBAR_WAVETABLE_LENGTH+1];

inline int16_t interpolate(int16_t v1, int16_t v2, uint32_t scale) {
    v1 *= 0xFFFF - scale;
    v2 *= scale;
    return (v1 + v2) >> 16;
}

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
  
}

