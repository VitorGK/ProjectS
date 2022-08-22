/*
  ==============================================================================

    OscData.h
    Created: 9 May 2022 9:30:18am
    Author:  Vitor Grechi Kuninari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OscData: public dsp::Oscillator<float> {
public:
    void prepareToPlay(const dsp::ProcessSpec &spec);
    void setOscFrequency(const int midiNoteNumber);
    void processNextBlock(dsp::AudioBlock<float>& audioBlock);
    void setWaveType(const int waveType);
    
private:
    
};
