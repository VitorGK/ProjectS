/*
  ==============================================================================

    OscComponent.h
    Created: 9 May 2022 9:30:57am
    Author:  Vitor Grechi Kuninari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OscComponent: public Component {
public:
    OscComponent(AudioProcessorValueTreeState& apvts);
    ~OscComponent() override;

    void paint (Graphics&) override;
    void resized() override;

private:
    ComboBox oscWaveTypeSelector;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveTypeSelectorAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
