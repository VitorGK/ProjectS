/*
  ==============================================================================

    KnobComponent.h
    Created: 29 Jun 2022 4:05:43pm
    Author:  Vitor Grechi Kuninari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class KnobComponent: public Component {
public:
    KnobComponent(AudioProcessorValueTreeState& apvts, String knobName, const float initValue);
    ~KnobComponent() override;
    
    void paint(Graphics&) override;
    void resized() override;
    
private:
    Slider knobSlider;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> knobSliderAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KnobComponent)
};
