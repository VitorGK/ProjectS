/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

//==============================================================================
ProjectSAudioProcessorEditor::ProjectSAudioProcessorEditor(ProjectSAudioProcessor& p):
AudioProcessorEditor(&p),
audioProcessor(p),
keyboardComponent(p.keyboardState, MidiKeyboardComponent::horizontalKeyboard),
samplerLoadButton("Load Sample"),
oscComponent(p.apvts),
oscAttackComponent(p.apvts, "OscAttack", 0.01f),
oscDecayComponent(p.apvts, "OscDecay", 0.5f),
oscSustainComponent(p.apvts, "OscSustain", 1.0f),
oscReleaseComponent(p.apvts, "OscRelease", 0.05f),
oscGainComponent(p.apvts, "OscGain", 1.0f),
distortionDriveComponent(p.apvts, "DistortionDrive", 1.0f),
delayFeedbackComponent(p.apvts, "DelayFeedback", 0.0f)
{
    setSize(1000, 800);
    
    samplerLoadButton.onClick = [&]() { p.loadFile(); };
    addAndMakeVisible(samplerLoadButton);
    
    addAndMakeVisible(oscComponent);
    addAndMakeVisible(oscAttackComponent);
    addAndMakeVisible(oscDecayComponent);
    addAndMakeVisible(oscSustainComponent);
    addAndMakeVisible(oscReleaseComponent);
    addAndMakeVisible(oscGainComponent);
    
    addAndMakeVisible(distortionDriveComponent);
    
    addAndMakeVisible(delayFeedbackComponent);
    
    samplerSampleLabel.setJustificationType(Justification::topLeft);
    addAndMakeVisible(samplerSampleLabel);
    
    oscAdsrLabel.setJustificationType(Justification::centredLeft);
    addAndMakeVisible(oscAdsrLabel);
    oscWaveTypeLabel.setJustificationType(Justification::centredLeft);
    addAndMakeVisible(oscWaveTypeLabel);
    
    oscAttackLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(oscAttackLabel);
    oscDecayLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(oscDecayLabel);
    oscSustainLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(oscSustainLabel);
    oscReleaseLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(oscReleaseLabel);
    oscGainLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(oscGainLabel);
    
    distortionDriveLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(distortionDriveLabel);
    
    delayFeedbackLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(delayFeedbackLabel);
    
    addAndMakeVisible(keyboardComponent);
    
    startTimer(400);
}

ProjectSAudioProcessorEditor::~ProjectSAudioProcessorEditor() {}

void ProjectSAudioProcessorEditor::paint(Graphics& g) {
    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    
    if (audioProcessor.isSampleLoaded) {
        samplerLoadButton.setAlpha(0);
        
        Path p;
        p.clear();
        audioPoints.clear();
        
        auto waveform = audioProcessor.getWaveform();
        auto ratio = waveform.getNumSamples() / (getWidth()-40);
        auto buffer = waveform.getReadPointer(0);
        
        // X Axis
        for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio) {
            audioPoints.push_back(buffer[sample]);
        }
        
        p.startNewSubPath(20, 255);
        
        // Y Axis
        for (int sample = 0; sample < audioPoints.size(); ++sample) {
            auto point = jmap<float>(audioPoints[sample], -1.0f, 1.0f, 255, 120);
            p.lineTo(sample+20, point);
        }
        
        g.strokePath(p, PathStrokeType(2));
    }
    
    Path adsr;
    adsr.clear();
    
    float adsrXOrigin = getWidth()/2;
    float adsrYOrigin = getHeight()-150;
    float adsrWidth = (getWidth()/2)-20;
    float adsrHeight = 290;
    
    float oscAttack = *audioProcessor.apvts.getRawParameterValue("OscAttack") * adsrWidth/30;
    float oscDecay = *audioProcessor.apvts.getRawParameterValue("OscDecay") * adsrWidth/30;
    float oscSustain = *audioProcessor.apvts.getRawParameterValue("OscSustain") * adsrHeight;
    float oscRelease = *audioProcessor.apvts.getRawParameterValue("OscRelease") * adsrWidth/30;
    
    adsr.startNewSubPath(adsrXOrigin, adsrYOrigin);
    adsr.lineTo(adsrXOrigin+oscAttack, adsrYOrigin-adsrHeight);
    adsr.lineTo(adsrXOrigin+oscAttack+oscDecay, adsrYOrigin-oscSustain);
    adsr.lineTo(adsrXOrigin+oscAttack+oscDecay+oscRelease, adsrYOrigin);
    
    g.strokePath(adsr, PathStrokeType(5));
    
    repaint();
}

void ProjectSAudioProcessorEditor::resized() {
    samplerSampleLabel.setBounds(20, 20, 100, 50);
    samplerLoadButton.setBounds(20, 40, 960, 250);
    
    oscComponent.setBounds(20, 340, oscComponent.getParentWidth(), oscComponent.getParentHeight());
    oscAttackComponent.setBounds(20, 420, oscAttackComponent.getParentWidth(), oscAttackComponent.getParentHeight());
    oscDecayComponent.setBounds(100, 420, oscDecayComponent.getParentWidth(), oscDecayComponent.getParentHeight());
    oscSustainComponent.setBounds(180, 420, oscSustainComponent.getParentWidth(), oscSustainComponent.getParentHeight());
    oscReleaseComponent.setBounds(260, 420, oscReleaseComponent.getParentWidth(), oscReleaseComponent.getParentHeight());
    oscGainComponent.setBounds(340, 420, oscGainComponent.getParentWidth(), oscGainComponent.getParentHeight());
    
    distortionDriveComponent.setBounds(20, 550, distortionDriveComponent.getParentWidth(), distortionDriveComponent.getParentHeight());
    
    delayFeedbackComponent.setBounds(100, 550, delayFeedbackComponent.getParentWidth(), delayFeedbackComponent.getParentHeight());
    
    oscWaveTypeLabel.setBounds(20, 300, 100, 50);
    oscAdsrLabel.setBounds(getWidth()/2, 300, 100, 50);
    oscAttackLabel.setBounds(20, 390, 80, 50);
    oscDecayLabel.setBounds(100, 390, 80, 50);
    oscSustainLabel.setBounds(180, 390, 80, 50);
    oscReleaseLabel.setBounds(260, 390, 80, 50);
    oscGainLabel.setBounds(340, 390, 80, 50);
    
    distortionDriveLabel.setBounds(20, 520, 80, 50);
    
    delayFeedbackLabel.setBounds(100, 520, 80, 50);
    
    auto r = getLocalBounds();
//    r.reduce(150, 0);
    keyboardComponent.setBounds(r.removeFromBottom(100));
}

void ProjectSAudioProcessorEditor::timerCallback() {
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}

bool ProjectSAudioProcessorEditor::isInterestedInFileDrag(const StringArray& files) {
    for (auto file: files) {
        if (file.contains(".wav") || file.contains(".mp3")) {
            return true;
        }
    }
    return false;
}

void ProjectSAudioProcessorEditor::filesDropped(const StringArray& files, int x, int y) {
    for (auto file: files) {
        if (isInterestedInFileDrag(files)) {
            audioProcessor.loadFile(file);
        }
    }
//    repaint();
}
