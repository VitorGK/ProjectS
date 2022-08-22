/*
  ==============================================================================

    SynthSound.h
    Created: 9 May 2022 9:29:20am
    Author:  Vitor Grechi Kuninari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct SynthSound: public SynthesiserSound {
public:
    bool appliesToNote(int midiNoteNumber) override;
    bool appliesToChannel(int midiChannel) override;
    
private:
    
};
