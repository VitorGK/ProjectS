/*
  ==============================================================================

    SynthSound.cpp
    Created: 9 May 2022 9:56:23am
    Author:  Vitor Grechi Kuninari

  ==============================================================================
*/

#include "SynthSound.h"

bool SynthSound::appliesToNote(int midiNoteNumber) { return true; }
bool SynthSound::appliesToChannel(int midiChannel) { return true; }
