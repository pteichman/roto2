// Copyright (c) 2015 Peter Teichman

#include <Arduino.h>

#include "voice_allocator.h"

voice_t VoiceAllocator::NoteOn(uint8_t note) {
  voice_t voice = find(note);
  if (voice != kNoVoice) {
    return voice;
  }

  // Find any unused voice.
  for (int i=0; i<kNumVoices; i++) {
    if (voices_[i] == kNoVoice) {
      voices_[i] = note;
      voice = i;
      break;
    }
  }

  // Find least recently used voice.
  if (voice == kNoVoice) {
    int i;
    for (i=kNumVoices-1; i>=0; i--) {
      if (lru_[i] != kNoVoice) {
        break;
      }
    }
    voice = lru_[i];
    voices_[i] = note;
  }

  touch(voice);
  return voice;
}

// Release note, return the voice it was on.
voice_t VoiceAllocator::NoteOff(uint8_t note) {
  voice_t voice = find(note);
  if (voice != kNoVoice) {
    voices_[voice] = kNoVoice;
    touch(voice);
  }
  return voice;
}

voice_t VoiceAllocator::find(uint8_t note) {
  voice_t voice = kNoVoice;
  for (int i=0; i<kNumVoices; i++) {
    if (voices_[i] == note) {
      voice = i;
    }
  }
  return voice;
}

void VoiceAllocator::touch(uint8_t voice) {
  int l=kNumVoices-1;
  int r=kNumVoices-1;

  while (l >= 0) {
    if (lru_[l] != voice) {
      lru_[r--] = lru_[l];
    }
    --l;
  }

  lru_[0] = voice;
}
