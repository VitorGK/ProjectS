/*
  ==============================================================================

    SynthVoice.cpp
    Created: 9 May 2022 9:29:13am
    Author:  Vitor Grechi Kuninari

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(SynthesiserSound* sound) {
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) {
    osc.setOscFrequency(midiNoteNumber);
    adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    if (allowTailOff) {
        adsr.noteOff();
    } else {
        clearCurrentNote();
    }
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {}

void SynthVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared);
    
    if (!isVoiceActive()) {
        return;
    }
    
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();
    
    dsp::AudioBlock<float> audioBlock = { synthBuffer };
    osc.processNextBlock(audioBlock);
    gain.process(dsp::ProcessContextReplacing<float>(audioBlock));
    
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        
        if (!adsr.isActive()) {
            clearCurrentNote();
        }
    }
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    adsr.setSampleRate(sampleRate);
    
    dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = outputChannels;
    spec.sampleRate = sampleRate;
    
    osc.prepareToPlay(spec);
    gain.prepare(spec);
    
    gain.setGainLinear(1.0f);
    
    isPrepared = true;
}

void SynthVoice::setADSR(const float attack, const float decay, const float sustain, const float release) {
    adsr.setADSR(attack, decay, sustain, release);
}

void SynthVoice::setGain(const float newValue) {
    gain.setGainLinear(newValue);
}

OscData& SynthVoice::getOscillator() { return osc; }
