/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EP491SaturationAudioProcessor::EP491SaturationAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
}

EP491SaturationAudioProcessor::~EP491SaturationAudioProcessor()
{
}

//==============================================================================
const juce::String EP491SaturationAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EP491SaturationAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EP491SaturationAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EP491SaturationAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EP491SaturationAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EP491SaturationAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EP491SaturationAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EP491SaturationAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String EP491SaturationAudioProcessor::getProgramName (int index)
{
    return {};
}

void EP491SaturationAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void EP491SaturationAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void EP491SaturationAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EP491SaturationAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void EP491SaturationAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    auto& distType = *apvts.getRawParameterValue("DISTTYPE");
    auto& distGain = *apvts.getRawParameterValue("DISTGAIN");
    auto& distLevel = *apvts.getRawParameterValue("DISTLEVEL");
    
    setDistortionType(distType, buffer, distGain, distLevel, getTotalNumOutputChannels());
}

//==============================================================================
bool EP491SaturationAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* EP491SaturationAudioProcessor::createEditor()
{
    return new EP491SaturationAudioProcessorEditor (*this);
}

//==============================================================================
void EP491SaturationAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EP491SaturationAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EP491SaturationAudioProcessor();
}

void EP491SaturationAudioProcessor::setDistortionType(const int choice, juce::AudioBuffer<float> &buffer, float gain, float level, int numChannels)
{
    switch (choice) {
        case 0:
            distortionOff(buffer, gain, level, numChannels);
            break;
        
        case 1:
            hardClip(buffer, gain, level, numChannels);
            break;
            
        case 2:
            softClip(buffer, gain, level, numChannels);
            break;
            
        default:
            jassertfalse;
            break;
    }
}

void EP491SaturationAudioProcessor::hardClip(juce::AudioBuffer<float>& buffer, float gain, float level, int numChannels)
{
    float* channelLeft = buffer.getWritePointer(0);
    float* channelRight = buffer.getWritePointer(1);

    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        channelLeft[sample] *= gain;
        channelRight[sample] *= gain;

        if (channelLeft[sample] > 1.0f)
        {
            channelLeft[sample] = 1.0f;
        } else if (channelLeft[sample] < -1.0f)
        {
            channelLeft[sample] =  -1.0f;
        }

        if (channelRight[sample] > 1.0f)
        {
            channelRight[sample] = 1.0f;
        } else if (channelRight[sample] < -1.0f)
        {
            channelRight[sample] =  -1.0f;
        }

        channelLeft[sample] *= level;
        channelRight[sample] *= level;
    }
}

void EP491SaturationAudioProcessor::softClip(juce::AudioBuffer<float>& buffer, float gain, float level, int numChannels)
{
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= juce::Decibels::decibelsToGain(gain);
            channelData[sample] = (2 / M_PI) * atan(channelData[sample]);

            channelData[sample] *= level;
        }
    }
}

void EP491SaturationAudioProcessor::distortionOff(juce::AudioBuffer<float>& buffer, float gain, float level, int numChannels)
{
    
}

juce::AudioProcessorValueTreeState::ParameterLayout EP491SaturationAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "DISTTYPE", 1 }, "Distortion Type", juce::StringArray { "Off", "Hard Clip", "Soft Clip" }, 2));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "DISTGAIN", 1}, "Distortion Gain", juce::NormalisableRange<float> { 0.01f, 25.0f, 0.01f, 0.6f }, 10.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "DISTLEVEL", 1}, "Distortion Level", juce::NormalisableRange<float> { 0.1f, 1.0f, 0.01f }, 1.0f));
    
    return { params.begin(), params.end() };
}
