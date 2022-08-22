/*
  ==============================================================================

    OscComponent.cpp
    Created: 9 May 2022 9:30:57am
    Author:  Vitor Grechi Kuninari

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(AudioProcessorValueTreeState& apvts) {
    oscWaveTypeSelectorAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "OscWaveType", oscWaveTypeSelector);
    
    StringArray waveTypes = StringArray("Sine", "Saw", "Square");
    oscWaveTypeSelector.addItemList(waveTypes, 1);
    oscWaveTypeSelector.setText(waveTypes[0]);
    addAndMakeVisible(oscWaveTypeSelector);
}

OscComponent::~OscComponent() {}

void OscComponent::paint(Graphics& g) {}

void OscComponent::resized() {
    oscWaveTypeSelector.setBounds(0, 0, 400, 40);
}
