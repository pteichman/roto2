// Copyright (c) 2015 Peter Teichman

#ifndef VOICE_ALLOCATOR_H
#define VOICE_ALLOCATOR_H

typedef uint8_t voice_t;

const uint8_t kNumVoices = 4;
const uint8_t kNoVoice = 0xFF;

class VoiceAllocator
{
  public:
  VoiceAllocator() {
  }

  void Init() {
    for (int i=0; i<kNumVoices; i++) {
      voices_[i] = kNoVoice;
      lru_[i] = kNoVoice;
    }
  }

  // Press down note, return voice to start playing.
  voice_t NoteOn(uint8_t note);

  // Release note, return the voice it was on.
  voice_t NoteOff(uint8_t note);

  private:
  voice_t voices_[kNumVoices];
  uint8_t lru_[kNumVoices];

  voice_t find(uint8_t note);
  void touch(uint8_t voice);
};

#endif
