/*
  ==============================================================================

    KnobComponent.cpp
    Created: 29 Jun 2022 4:05:43pm
    Author:  Vitor Grechi Kuninari

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KnobComponent.h"

//==============================================================================
KnobComponent::KnobComponent(AudioProcessorValueTreeState& apvts, String knobName, const float initValue) {
    knobSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, knobName, knobSlider);
    
    knobSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    knobSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    knobSlider.setValue(initValue);
    addAndMakeVisible(knobSlider);
}

KnobComponent::~KnobComponent() {}

void KnobComponent::paint(Graphics& g) {}

void KnobComponent::resized() {
    knobSlider.setBounds(0, 0, 80, 80);
}
