/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/OscComponent.h"
#include "UI/KnobComponent.h"

//==============================================================================
/**
*/
class ProjectSAudioProcessorEditor: public AudioProcessorEditor,
                                    private Timer,
                                    public FileDragAndDropTarget
{
public:
    ProjectSAudioProcessorEditor(ProjectSAudioProcessor&);
    ~ProjectSAudioProcessorEditor() override;
    
    void paint (Graphics&) override;
    void resized() override;
//====================================================================================================
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    
private:
    void timerCallback() override;
    
    ProjectSAudioProcessor& audioProcessor;
    
    MidiKeyboardComponent keyboardComponent;
    
    TextButton samplerLoadButton;
    std::vector<float> audioPoints;
    bool shouldBePainting = false;
    
    OscComponent oscComponent;
    KnobComponent oscAttackComponent;
    KnobComponent oscDecayComponent;
    KnobComponent oscSustainComponent;
    KnobComponent oscReleaseComponent;
    KnobComponent oscGainComponent;
    
    KnobComponent distortionDriveComponent;
    
    KnobComponent delayFeedbackComponent;
    
    Label samplerSampleLabel = Label("samplerSampleLabel", "SAMPLE");
    
    Label oscWaveTypeLabel = Label("oscWaveTypeLabel", "WAVE TYPE");
    Label oscAdsrLabel = Label("oscAdsrLabel", "ADSR");
    Label oscAttackLabel = Label("OscAttackLabel", "ATTACK");
    Label oscDecayLabel = Label("OscDecayLabel", "DECAY");
    Label oscSustainLabel = Label("OscSustainLabel", "SUSTAIN");
    Label oscReleaseLabel = Label("OscReleaseLabel", "RELEASE");
    Label oscGainLabel = Label("OscGainLabel", "GAIN");
    
    Label distortionDriveLabel = Label("DistortionDriveLabel", "DISTORTION");
    
    Label delayFeedbackLabel = Label("DelayFeedbackLabel", "DELAY");
    
//====================================================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectSAudioProcessorEditor)
};
