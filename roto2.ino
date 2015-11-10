// Copyright (c) 2015 Peter Teichman

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "voice_allocator.h"

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=339,52
AudioSynthWaveformSine   sine2;          //xy=339,88
AudioSynthWaveformSine   sine3;          //xy=339,124
AudioSynthWaveformSine   sine4;          //xy=340,174
AudioSynthWaveformSine   sine5;          //xy=340,209
AudioSynthWaveformSine   sine6;          //xy=340,244
AudioSynthWaveformSine   sine7;          //xy=340,291
AudioSynthWaveformSine   sine8;          //xy=341,327
AudioSynthWaveformSine   sine9;         //xy=341,412
AudioSynthWaveformSine   sine10;         //xy=341,483
AudioSynthWaveformSine   sine11;          //xy=342,363
AudioSynthWaveformSine   sine12;         //xy=342,447
AudioSynthWaveformSine   sine13;         //xy=342,447
AudioSynthWaveformSine   sine14;         //xy=342,447
AudioSynthWaveformSine   sine15;         //xy=342,447
AudioSynthWaveformSine   sine16;         //xy=342,447
AudioMixer4              mixer1;         //xy=459,325
AudioMixer4              mixer2;         //xy=461,90
AudioMixer4              mixer3;         //xy=461,208
AudioMixer4              mixer4;         //xy=461,444
AudioEffectEnvelope      envelope1;      //xy=591,90
AudioEffectEnvelope      envelope2;      //xy=591,208
AudioEffectEnvelope      envelope3;      //xy=593,326
AudioEffectEnvelope      envelope4;      //xy=596,444
AudioMixer4              mixer5;         //xy=759,274
AudioOutputI2S           i2s1;           //xy=887,273
AudioConnection          patchCord1(sine1, 0, mixer1, 0);
AudioConnection          patchCord2(sine2, 0, mixer1, 1);
AudioConnection          patchCord3(sine3, 0, mixer1, 2);
AudioConnection          patchCord4(sine4, 0, mixer1, 3);
AudioConnection          patchCord5(sine5, 0, mixer2, 0);
AudioConnection          patchCord6(sine6, 0, mixer2, 1);
AudioConnection          patchCord7(sine7, 0, mixer2, 2);
AudioConnection          patchCord8(sine8, 0, mixer2, 3);
AudioConnection          patchCord9(sine9, 0, mixer3, 0);
AudioConnection          patchCord10(sine10, 0, mixer3, 1);
AudioConnection          patchCord11(sine11, 0, mixer3, 2);
AudioConnection          patchCord12(sine12, 0, mixer3, 3);
AudioConnection          patchCord13(sine13, 0, mixer4, 0);
AudioConnection          patchCord14(sine14, 0, mixer4, 1);
AudioConnection          patchCord15(sine15, 0, mixer4, 2);
AudioConnection          patchCord16(sine16, 0, mixer4, 3);
AudioConnection          patchCord17(mixer1, envelope1);
AudioConnection          patchCord18(mixer2, envelope2);
AudioConnection          patchCord19(mixer3, envelope3);
AudioConnection          patchCord20(mixer4, envelope4);
AudioConnection          patchCord21(envelope1, 0, mixer5, 0);
AudioConnection          patchCord22(envelope2, 0, mixer5, 1);
AudioConnection          patchCord23(envelope3, 0, mixer5, 2);
AudioConnection          patchCord24(envelope4, 0, mixer5, 3);
AudioConnection          patchCord25(mixer5, 0, i2s1, 0);
AudioConnection          patchCord26(mixer5, 0, i2s1, 1);
AudioControlSGTL5000     audioShield;     //xy=132,494
// GUItool: end automatically generated code

VoiceAllocator voices;

void setup() {
  Serial.begin(115200);

  AudioMemory(20);
  audioShield.enable();
  audioShield.volume(0.5);

  voices.Init();
  initVoice(1);
  initVoice(2);
  initVoice(3);
  initVoice(4);

  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleVelocityChange(OnVelocityChange);
  usbMIDI.setHandleControlChange(OnControlChange);
  usbMIDI.setHandleProgramChange(OnProgramChange);
  usbMIDI.setHandleAfterTouch(OnAfterTouch);
  usbMIDI.setHandlePitchChange(OnPitchChange);
}

void loop() {
  usbMIDI.read(); // USB MIDI receive
}

void initVoice(byte voice) {
  switch (voice) {
    case 1:
      initSine(&sine1);
      initSine(&sine2);
      initSine(&sine3);
      initSine(&sine4);
      initEnvelope(&envelope1);
      initMixer(&mixer1);
      break;
    case 2:
      initSine(&sine5);
      initSine(&sine6);
      initSine(&sine7);
      initSine(&sine8);
      initEnvelope(&envelope2);
      initMixer(&mixer2);
      break;
    case 3:
      initSine(&sine9);
      initSine(&sine10);
      initSine(&sine11);
      initSine(&sine12);
      initEnvelope(&envelope3);
      initMixer(&mixer3);
      break;
    case 4:
      initSine(&sine13);
      initSine(&sine14);
      initSine(&sine15);
      initSine(&sine16);
      initEnvelope(&envelope4);
      initMixer(&mixer4);
      break;
  }
}

void initSine(AudioSynthWaveformSine *sine) {
  sine->amplitude(1.0);
}

void initEnvelope(AudioEffectEnvelope *env) {
  env->attack(2.0);
  env->sustain(1.0);
  env->release(2.0);
}

void initMixer(AudioMixer4 *mix) {
  mix->gain(0, 0.0625);
  mix->gain(1, 0.0625);
  mix->gain(2, 0.0625);
  mix->gain(3, 0.0625);
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  Serial.print("Note On, ch=");
  Serial.print(channel, DEC);
  Serial.print(", note=");
  Serial.print(note, DEC);
  Serial.print(", velocity=");
  Serial.print(velocity, DEC);
  Serial.println();

  voice_t voice = voices.NoteOn(note);
  float fund = note2freq(note);
  
  switch (voice) {
    case 0:
      sine1.frequency(fund);
      sine2.frequency(fund * 0.5);
      sine3.frequency(fund * 1.5);
      sine4.frequency(fund * 2.0);
      envelope1.noteOn();
      break;
    case 1:
      sine5.frequency(fund);
      sine6.frequency(fund * 0.5);
      sine7.frequency(fund * 1.5);
      sine8.frequency(fund * 2.0);
      envelope2.noteOn();
      break;
    case 2:
      sine9.frequency(fund);
      sine10.frequency(fund * 0.5);
      sine11.frequency(fund * 1.5);
      sine12.frequency(fund * 2.0);
      envelope3.noteOn();
      break;
    case 3:
      sine13.frequency(fund);
      sine14.frequency(fund * 0.5);
      sine15.frequency(fund * 1.5);
      sine16.frequency(fund * 2.0);
      envelope4.noteOn();
      break;
  }
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  Serial.print("Note Off, ch=");
  Serial.print(channel, DEC);
  Serial.print(", note=");
  Serial.print(note, DEC);
  Serial.print(", velocity=");
  Serial.print(velocity, DEC);
  Serial.println();

  voice_t voice = voices.NoteOff(note);
  switch (voice) {
    case 0:
      envelope1.noteOff();
      break;
    case 1:
      envelope2.noteOff();
      break;
    case 2:
      envelope3.noteOff();
      break;
    case 3:
      envelope4.noteOff();
      break;
  }
}

void OnVelocityChange(byte channel, byte note, byte velocity) {
  Serial.print("Velocity Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", note=");
  Serial.print(note, DEC);
  Serial.print(", velocity=");
  Serial.print(velocity, DEC);
  Serial.println();
}

void OnControlChange(byte channel, byte control, byte value) {
  Serial.print("Control Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", control=");
  Serial.print(control, DEC);
  Serial.print(", value=");
  Serial.print(value, DEC);
  Serial.println();
}

void OnProgramChange(byte channel, byte program) {
  Serial.print("Program Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", program=");
  Serial.print(program, DEC);
  Serial.println();
}

void OnAfterTouch(byte channel, byte pressure) {
  Serial.print("After Touch, ch=");
  Serial.print(channel, DEC);
  Serial.print(", pressure=");
  Serial.print(pressure, DEC);
  Serial.println();
}

void OnPitchChange(byte channel, int pitch) {
  Serial.print("Pitch Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", pitch=");
  Serial.print(pitch, DEC);
  Serial.println();
}

float note2freq(byte note) {
  float tmp = (note-69) / 12.0;
  return 440.0 * pow(2.0, tmp);
}

