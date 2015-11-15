// Copyright (c) 2015 Peter Teichman

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "drawbar_osc.h"
#include "voice_allocator.h"

// GUItool: begin automatically generated code
DrawbarOsc               osc1;
DrawbarOsc               osc2;
DrawbarOsc               osc3;
DrawbarOsc               osc4;
AudioEffectEnvelope      envelope1;      //xy=591,90
AudioEffectEnvelope      envelope2;      //xy=591,208
AudioEffectEnvelope      envelope3;      //xy=593,326
AudioEffectEnvelope      envelope4;      //xy=596,444
AudioMixer4              mixer5;         //xy=759,274
AudioOutputI2S           i2s1;           //xy=887,273
AudioConnection          patchCord17(osc1, envelope1);
AudioConnection          patchCord18(osc2, envelope2);
AudioConnection          patchCord19(osc3, envelope3);
AudioConnection          patchCord20(osc4, envelope4);
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
    audioShield.volume(0.7);

    voices.Init();
    initVoice(0);
    initVoice(1);
    initVoice(2);
    initVoice(3);

    osc1.AllDrawbars(0x888800000);
    mixer5.gain(0, 0.25);
    mixer5.gain(1, 0.25);
    mixer5.gain(2, 0.25);
    mixer5.gain(3, 0.25);

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
    case 0:
        osc1.Begin(0.0, 1.0, 0x000000000);
        initEnvelope(&envelope1);
        break;
    case 1:
        osc2.Begin(0.0, 1.0, 0x000000000);
        initEnvelope(&envelope2);
        break;
    case 2:
        osc3.Begin(0.0, 1.0, 0x000000000);
        initEnvelope(&envelope3);
        break;
    case 3:
        osc4.Begin(0.0, 1.0, 0x000000000);
        initEnvelope(&envelope4);
        break;
    }
}

void initEnvelope(AudioEffectEnvelope *env) {
    env->attack(2.0);
    env->sustain(1.0);
    env->release(2.0);
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
        osc1.Fundamental(fund);
        envelope1.noteOn();
        break;
    case 1:
        osc2.Fundamental(fund);
        envelope2.noteOn();
        break;
    case 2:
        osc3.Fundamental(fund);
        envelope3.noteOn();
        break;
    case 3:
        osc4.Fundamental(fund);
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

    if (control >= 16 && control < 25) {
        int drawbar = control - 16;
        float val = (float)value / 127.0;
        osc1.Drawbar(drawbar, val);
    }
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
