/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "SynthVoice.h"

//==============================================================================
/**
*/
class ProjectSAudioProcessor: public AudioProcessor {
public:
    ProjectSAudioProcessor();
    ~ProjectSAudioProcessor() override;
    
    const String getName() const override;
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    
    double getTailLengthSeconds() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    #endif
    
    bool isSampleLoaded = false;
    
    void fillBuffer(int channel, int bufferSize, int delayBufferSize, const float* channelData);
    void readBuffer(AudioBuffer<float>& buffer, int sampleRate, int channel, int bufferSize, int delayBufferSize, const float* channelData, const float* delayChannelData, float delayFeedback);
//    void feedbackDelay(AudioBuffer<float>& buffer, int channel, int bufferSize, int delayBufferSize, const float* dryBuffer, float delayFeedback);
    
//====================================================================================================
    void loadFile();
    void loadFile(const String& path);
    int getNumSamplerSounds();
    AudioBuffer<float>& getWaveform();
    
    AudioProcessorValueTreeState apvts;
    MidiKeyboardState keyboardState;
    
private:
    void initSampler();
    void initSynth();
    AudioProcessorValueTreeState::ParameterLayout createParams();
    
    std::unique_ptr<FileChooser> fileChooser;
    AudioFormatManager formatManager;
    AudioFormatReader* formatReader = nullptr;
    
    Synthesiser sampler;
    int samplerVoices = 32;
    AudioBuffer<float> waveform;
    
    Synthesiser synth;
    int synthVoices = 32;
    
    float maxOutputLevel = 0.25;
//    DelayEffect<float> delayEffect = *this;
    
    AudioBuffer<float> delayBuffer;
    int writePosition = 0;
    
    AudioPlayHead* playHead;
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
//====================================================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectSAudioProcessor)
};
