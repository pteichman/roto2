// Copyright (c) 2015 Peter Teichman

#include <Arduino.h>

#include "voice_allocator.h"

voice_t VoiceAllocator::NoteOn(uint8_t tag) {
    // Find the least recently used voice.
    voice_t voice = kNoVoice;
    int lru = kNoVoice;

    for (int i=0; i<kNumVoices; i++) {
        if (lru_[i] < lru) {
            voice = i;
            lru = lru_[i];
        }
    }

    if (voice != kNoVoice) {
        voices_[voice] = tag;
        lru_[voice] = tick_++;
    }

    return voice;
}

voice_t VoiceAllocator::NoteOff(uint8_t tag) {
    for (int i=0; i<kNumVoices; i++) {
        if (voices_[i] == tag) {
            voices_[i] = kNoVoice;
            lru_[i] = tick_++;
            return i;
        }
    }
    return kNoVoice;
}
