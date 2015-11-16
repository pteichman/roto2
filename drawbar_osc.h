// Copyright (c) 2015 Peter Teichman

#ifndef DRAWBAR_OSC_H
#define DRAWBAR_OSC_H

#include <Audio.h>

#define DRAWBAR_WAVETABLE_LENGTH (768)
extern int16_t DrawbarWave[DRAWBAR_WAVETABLE_LENGTH+1];

class DrawbarOsc : public AudioStream {
 public:
    DrawbarOsc() : AudioStream(0, NULL) {
    }

    void Init() {
        tone_phase = 0;
        tone_incr = 0;

        amp = 0;
        for (int i=0; i<9; i++) {
            drawbars[i] = 0;
        }

        for (int i=0; i<DRAWBAR_WAVETABLE_LENGTH; i++) {
            DrawbarWave[i] = 0;
        }
        DrawbarWave[DRAWBAR_WAVETABLE_LENGTH] = DrawbarWave[0];
    }

    void Begin(float freq, float amp, long long harmonics) {
        Init();
        Fundamental(freq);
        Amplitude(amp);
        AllDrawbars(harmonics);
    }

    // AllDrawbars takes a long long, so that drawbar settings can be
    // made inline. Each nibble turns into a single drawbar value, so
    // e.g. 0x888800000 sets the first four drawbars to 8 and the rest
    // to zero.
    void AllDrawbars(long long harmonics) {
        drawbars[8] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[7] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[6] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[5] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[4] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[3] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[2] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[1] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[0] = 28*(harmonics & 0xf);

        Serial.println("Rebuilding drawbars");
        Rebuild();
    }

    void Drawbar(int n, float v) {
        if (v < 0.0) {
            v = 0.0;
        } else if (v > 1.0) {
            v = 1.0;
        }
        drawbars[n] = (uint8_t)(256*v);

        Rebuild();
    }

    void Fundamental(float freq) {
        if (freq < 0.0) {
            freq = 0.0;
        } else if (freq > AUDIO_SAMPLE_RATE_EXACT / 2) {
            freq = AUDIO_SAMPLE_RATE_EXACT / 2;
        }
        tone_incr = freq * (0x10000000LL/AUDIO_SAMPLE_RATE_EXACT) + 0.5;
    }

    void Amplitude(float v) {
        if (v < 0.0) {
            v = 0.0;
        } else if (v > 1.0) {
            v = 1.0;
        }

        if ((amp == 0) && v) {
            tone_phase = 0;
        }

        amp = (uint16_t)(32767.0*v);
    }

    void update(void);

 private:
    uint8_t drawbars[9];
    uint16_t amp;
    uint32_t tone_phase;
    uint32_t tone_width;
    uint32_t tone_incr;

    virtual void Rebuild(void);
};

#endif
