/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ProjectSAudioProcessor::ProjectSAudioProcessor():
#ifndef JucePlugin_PreferredChannelConfigurations
AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
    .withInput("Input", AudioChannelSet::stereo(), true)
#endif
    .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
),
#endif
apvts(*this, nullptr, "apvts", createParams())
{
    formatManager.registerBasicFormats();
    initSampler();
    initSynth();
}

ProjectSAudioProcessor::~ProjectSAudioProcessor() {
//    formatReader->~AudioFormatReader();
}

const String ProjectSAudioProcessor::getName() const { return JucePlugin_Name; }

// MARK: - PREPARE TO PLAY
void ProjectSAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    String message;
    message << "Preparing to play audio...\n";
    message << "sampleRate = " << sampleRate << "\n";
    message << "samplesPerBlock = " << samplesPerBlock;
    Logger::getCurrentLogger()->writeToLog(message);
    
    sampler.setCurrentPlaybackSampleRate(sampleRate);
    
    synth.setCurrentPlaybackSampleRate(sampleRate);
    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
    
//    delayEffect.prepareToPlay(sampleRate, samplesPerBlock);
    Logger::getCurrentLogger()->writeToLog("Audio ready to play...");
}

// RELEASE RESOURCES
void ProjectSAudioProcessor::releaseResources() {
    Logger::getCurrentLogger()->writeToLog("Releasing audio resources...");
    Logger::getCurrentLogger()->writeToLog("Audio resources released...");
}

// MARK: - PROCESS BLOCK
void ProjectSAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
    int bufferSize = buffer.getNumSamples();
    int totalNumInputChannels = getTotalNumInputChannels();
    int totalNumOutputChannels = getTotalNumOutputChannels();
    
    // CLEAR BUFFER
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }
    
    // KEYBOARD
    keyboardState.processNextMidiBuffer(midiMessages, 0, bufferSize, true);
    
    // MARK: - SAMPLER
    sampler.renderNextBlock(buffer, midiMessages, 0, bufferSize);
    
    // MARK: - SYNTH
    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            auto& oscAttack = *apvts.getRawParameterValue("OscAttack");
            auto& oscDecay = *apvts.getRawParameterValue("OscDecay");
            auto& oscSustain = *apvts.getRawParameterValue("OscSustain");
            auto& oscRelease = *apvts.getRawParameterValue("OscRelease");
            
            auto& oscGain = *apvts.getRawParameterValue("OscGain");
            
            auto& oscWaveType = *apvts.getRawParameterValue("OscWaveType");
            
            voice->setADSR(oscAttack.load(), oscDecay.load(), oscSustain.load(), oscRelease.load());
            voice->setGain(oscGain.load() * maxOutputLevel);
            voice->getOscillator().setWaveType(oscWaveType.load());
        }
    }
    synth.renderNextBlock(buffer, midiMessages, 0, bufferSize);
    
    // MARK: - DISTORTION
    auto& distortionDrive = *apvts.getRawParameterValue("DistortionDrive");
    for (int channel = totalNumInputChannels; channel < totalNumOutputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            *channelData *= distortionDrive.load();
            
            // HARD CLIP
//            if (*channelData > 1.0f) {
//                *channelData = 1.0f;
//            } else if (*channelData < -1.0f) {
//                *channelData = -1.0f;
//            }
            
            // SOFT CLIP
            *channelData = (2.f / MathConstants<float>::pi) * atan(*channelData);
            
            ++channelData;
        }
    }
//    distortionDrive.setDrive(distortionDrive.load());
//    distortionDrive.process(buffer, bufferSize);
    
    // MARK: - DELAY
    auto& delayFeedback = *apvts.getRawParameterValue("DelayFeedback");
//    delayEffect.setFeedback(delayFeedback.load());
//    delayEffect.process(buffer, bufferSize);
}


double ProjectSAudioProcessor::getTailLengthSeconds() const { return 0.0; }

// MIDI
bool ProjectSAudioProcessor::acceptsMidi() const {
    #if JucePlugin_WantsMidiInput
    return true;
    #else
    return false;
    #endif
}

bool ProjectSAudioProcessor::producesMidi() const {
    #if JucePlugin_ProducesMidiOutput
    return true;
    #else
    return false;
    #endif
}

bool ProjectSAudioProcessor::isMidiEffect() const {
    #if JucePlugin_IsMidiEffect
    return true;
    #else
    return false;
    #endif
}

// EDITOR
AudioProcessorEditor* ProjectSAudioProcessor::createEditor() { return new ProjectSAudioProcessorEditor (*this); }
bool ProjectSAudioProcessor::hasEditor() const { return true; }

// SYSTEM
int ProjectSAudioProcessor::getNumPrograms() { return 1; }
int ProjectSAudioProcessor::getCurrentProgram() { return 0; }
void ProjectSAudioProcessor::setCurrentProgram(int index) {}
const String ProjectSAudioProcessor::getProgramName(int index) { return {}; }
void ProjectSAudioProcessor::changeProgramName(int index, const String& newName) {}

// STATE INFO
void ProjectSAudioProcessor::getStateInformation (juce::MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ProjectSAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

// BUSES LAYOUT SUPPORT
#ifndef JucePlugin_PreferredChannelConfigurations
bool ProjectSAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
    #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo()) {
        return false;
    }
    // This checks if the input layout matches the output layout
    #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) {
        return false;
    }
    #endif
    return true;
    #endif
}
#endif

AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new ProjectSAudioProcessor(); }

//====================================================================================================
void ProjectSAudioProcessor::loadFile() {
    sampler.clearSounds();
    
    fileChooser = std::make_unique<FileChooser>("Choose a sample to load...");
    
    fileChooser->launchAsync(FileBrowserComponent::canSelectFiles, [this](const FileChooser& fileChooser) {
        File file = fileChooser.getResult();
        
        if (file.exists()) {
            formatReader = formatManager.createReaderFor(file);
            
            auto sampleLength = static_cast<int>(formatReader->lengthInSamples);
            waveform.setSize(1, sampleLength);
            formatReader->read(&waveform, 0, sampleLength, 0, true, false);
            
            BigInteger allNotes;
            allNotes.setRange(0, 128, true);
            
            sampler.addSound(new SamplerSound("Sample", *formatReader, allNotes, 60, 0, 0, 10));
            isSampleLoaded = true;
        }
    });
}

void ProjectSAudioProcessor::loadFile(const String& path) {
    sampler.clearSounds();
    
    File file = File(path);
    if (file.exists()) {
        formatReader = formatManager.createReaderFor(file);
        
        auto sampleLength = static_cast<int>(formatReader->lengthInSamples);
        waveform.setSize(1, sampleLength);
        formatReader->read(&waveform, 0, sampleLength, 0, true, false);
        
        BigInteger allNotes;
        allNotes.setRange(0, 128, true);
        
        sampler.addSound(new SamplerSound("Sample", *formatReader, allNotes, 60, 0, 0, 10));
        isSampleLoaded = true;
    }
}

int ProjectSAudioProcessor::getNumSamplerSounds() { return sampler.getNumSounds(); }
AudioBuffer<float>& ProjectSAudioProcessor::getWaveform() { return waveform; }

void ProjectSAudioProcessor::initSampler() {
    for (int i = 0; i < samplerVoices; ++i) {
        sampler.addVoice(new SamplerVoice());
    }
    sampler.clearSounds();
}

void ProjectSAudioProcessor::initSynth() {
    for (int i = 0; i < synthVoices; ++i) {
        synth.addVoice(new SynthVoice());
    }
    synth.clearSounds();
    synth.addSound(new SynthSound());
}

AudioProcessorValueTreeState::ParameterLayout ProjectSAudioProcessor::createParams() {
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<AudioParameterFloat>("OscAttack", "Osc Attack", NormalisableRange<float>(0.0f, 10.0f, 0.01f), 0.01f));
    params.push_back(std::make_unique<AudioParameterFloat>("OscDecay", "Osc Decay", NormalisableRange<float>(0.0f, 10.0f, 0.01f), 0.5f));
    params.push_back(std::make_unique<AudioParameterFloat>("OscSustain", "Osc Sustain", NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("OscRelease", "Osc Release", NormalisableRange<float>(0.0f, 10.0f, 0.01f), 0.05f));
    
    params.push_back(std::make_unique<AudioParameterFloat>("OscGain", "Osc Gain", NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f));
    
    params.push_back(std::make_unique<AudioParameterChoice>("OscWaveType", "Osc Wave Type", StringArray("Sine", "Saw", "Square"), 0));
    
    params.push_back(std::make_unique<AudioParameterFloat>("DistortionDrive", "Distortion Drive", NormalisableRange<float>(1.0f, 100.0f, 0.01f), 1.0f));
    
    params.push_back(std::make_unique<AudioParameterFloat>("DelayFeedback", "Delay Feedback", NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));
    
    return { params.begin(), params.end() };
}
