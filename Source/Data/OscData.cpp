/*
  ==============================================================================

    OscData.cpp
    Created: 9 May 2022 9:30:18am
    Author:  Vitor Grechi Kuninari

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(const dsp::ProcessSpec &spec) {
    prepare(spec);
}

void OscData::setOscFrequency(const int midiNoteNumber) {
    setFrequency(MidiMessage::getMidiNoteInHertz(midiNoteNumber));
}

void OscData::processNextBlock(dsp::AudioBlock<float>& audioBlock) {
    process(dsp::ProcessContextReplacing<float>(audioBlock));
}

void OscData::setWaveType(const int waveType) {
    switch (waveType) {
        case 0:
            initialise([](double x) { return sin(x); });
            break;
        case 1:
            initialise([](double x) { return x/MathConstants<double>::pi; });
            break;
        case 2:
            initialise([](double x) { return x < 0.0f ? -1.0f : 1.0f; });
            break;
        default:
            jassertfalse;
            break;
    }
}
