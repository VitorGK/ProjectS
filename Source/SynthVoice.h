/*
  ==============================================================================

    SynthVoice.h
    Created: 9 May 2022 9:29:13am
    Author:  Vitor Grechi Kuninari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/OscData.h"
#include "Data/AdsrData.h"

struct SynthVoice: public SynthesiserVoice {
public:
    bool canPlaySound(SynthesiserSound*) override;
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void setADSR(const float attack, const float decay, const float sustain, const float release);
    void setGain(const float newValue);
    OscData& getOscillator();
    
private:
    bool isPrepared = false;
    AudioBuffer<float> synthBuffer;
    OscData osc;
    AdsrData adsr;
    dsp::Gain<float> gain;
};
