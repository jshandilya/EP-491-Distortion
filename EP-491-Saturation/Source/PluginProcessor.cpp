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
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = getSampleRate();
    spec.numChannels = getTotalNumOutputChannels();

    filter.prepare (spec);
    iirFilter.prepare (spec);
    
    reset();
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
    
    auto& input = *apvts.getRawParameterValue ("INPUT");
    auto& output = *apvts.getRawParameterValue ("OUTPUT");
    
    auto& distType = *apvts.getRawParameterValue ("DISTTYPE");
    auto& distGain = *apvts.getRawParameterValue ("DISTGAIN");
    auto& distLevel = *apvts.getRawParameterValue ("DISTLEVEL");
    
    auto& distType2 = *apvts.getRawParameterValue ("DISTTYPE2");
    auto& distGain2 = *apvts.getRawParameterValue ("DISTGAIN2");
    auto& distLevel2 = *apvts.getRawParameterValue ("DISTLEVEL2");

    auto& filterTypeProcess = *apvts.getRawParameterValue("FILTERTYPE");
    auto& cutoff = *apvts.getRawParameterValue("FILTERFREQ");
    auto& res = *apvts.getRawParameterValue("FILTERRES");
    auto& filterPos = *apvts.getRawParameterValue("FILTERPOS");
    
    auto& boomType = *apvts.getRawParameterValue("BOOMTYPE");
    auto& boomGain = *apvts.getRawParameterValue("BOOM");
    auto& boomFreq = *apvts.getRawParameterValue("BOOMFREQ");
    
    float diodeFreq = 200.f;
    
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= input;
        }
    }
    
    if (filterPos == 0)
    {
        setFilter (buffer, cutoff, res, filterTypeProcess);
        
        setDistortionType (distType, buffer, distGain, distLevel, diodeFreq, getSampleRate(), totalNumOutputChannels);
        setDistortionType (distType2, buffer, distGain2, distLevel2, diodeFreq, getSampleRate(), totalNumOutputChannels);
    }
    
    else if (filterPos == 1)
    {
        setDistortionType (distType, buffer, distGain, distLevel, diodeFreq, getSampleRate(), totalNumOutputChannels);

        setFilter (buffer, cutoff, res, filterTypeProcess);
        
        setDistortionType (distType2, buffer, distGain2, distLevel2, diodeFreq, getSampleRate(), totalNumOutputChannels);
    }
    
    else
    {
        setDistortionType (distType, buffer, distGain, distLevel, diodeFreq, getSampleRate(), totalNumOutputChannels);
        setDistortionType (distType2, buffer, distGain2, distLevel2, diodeFreq, getSampleRate(), totalNumOutputChannels);
        
        setFilter (buffer, cutoff, res, filterTypeProcess);
    }
    
    if (boomType == 1 )
    {
        boost (buffer, boomGain, boomFreq, getSampleRate(), totalNumOutputChannels);
    }
    
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= output;
        }
    }
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
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void EP491SaturationAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EP491SaturationAudioProcessor();
}

void EP491SaturationAudioProcessor::setDistortionType(const int choice, juce::AudioBuffer<float> &buffer, float gain, float level, float freq, double sampleRate, int numChannels)
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
            
        case 3:
            fuzz(buffer, gain, level, numChannels);
            break;
            
        case 4:
            sine(buffer, gain, level, numChannels);
            break;
        
        case 5:
            diode(buffer, freq, gain, level, sampleRate, numChannels);
            break;
            
        case 6:
            tanh(buffer, gain, level, numChannels);
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

void EP491SaturationAudioProcessor::fuzz(juce::AudioBuffer<float>& buffer, float gain, float level, int numChannels)
{
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            if (channelData[sample] < 0.0f)
            {
                channelData[sample] *= -1.0f;
            }
            
            channelData[sample] *= gain;
            
            if (channelData[sample] > 1.0f)
            {
                channelData[sample] = 1.0f;
            }
            
            channelData[sample] *= level;
        }
    }
}

void EP491SaturationAudioProcessor::sine(juce::AudioBuffer<float> &buffer, float gain, float level, int numChannels)
{
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= juce::Decibels::decibelsToGain(gain);
            
            channelData[sample] = std::sin (channelData[sample]);
            
            channelData[sample] *= level;
        }
    }
}

void EP491SaturationAudioProcessor::diode(juce::AudioBuffer<float>& buffer, float freq, float gain, float level, double sampleRate, int numChannels)
{
    auto g = std::tan (M_PI * freq / sampleRate);
    auto G = g / (1 + g);
    
    const auto numIterations = 8;
    const auto Is = 14.11e-9f;
    const auto mu = 1.984f;
    const auto Vt = 26e-3f;
    const auto R = 2.2e3;
    
    auto y = 0.f;
    auto s1 = 0.f;

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= juce::Decibels::decibelsToGain(gain);
            const auto p = G * (channelData[sample] - s1) + s1;
            
            for (auto k = 0; k < numIterations; k++)
            {
                auto J = p - (2 * G*R*Is) * std::sinh (y / mu / Vt) - y;
                auto dJ = -1 - G * 2 * R*Is / mu / Vt * std::cosh (y / mu / Vt);
                auto delta = -J / dJ;
                y = y + delta;
            }
            
            channelData[sample] = y;
            
            auto v = y - s1;
            s1 = y + v;
            
            channelData[sample] *= level;
        }
    }
}

void EP491SaturationAudioProcessor::tanh(juce::AudioBuffer<float>& buffer, float gain, float level, int numChannels)
{
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= juce::Decibels::decibelsToGain(gain);
            
            channelData[sample] = std::tanh(channelData[sample]);
            
            channelData[sample] *= level;
        }
    }
}

void EP491SaturationAudioProcessor::distortionOff(juce::AudioBuffer<float>& buffer, float gain, float level, int numChannels)
{

}

void EP491SaturationAudioProcessor::reset()
{
    filter.reset();
    iirFilter.reset();
}

void EP491SaturationAudioProcessor::setType(int choice)
{
    using fType = juce::dsp::StateVariableTPTFilterType;
    
    switch (choice)
    {
        case 0:
            filter.setType (fType::lowpass);
            break;
            
        case 1:
            filter.setType (fType::bandpass);
            break;
            
        case 2:
            filter.setType (fType::highpass);
            break;
            
        default:
            break;
    }
}

void EP491SaturationAudioProcessor::setFilter(juce::AudioBuffer<float> &buffer, float cutoff, float res, int filterType)
{
    setType (filterType);
    filter.setCutoffFrequency (cutoff);
    filter.setResonance (res);
    
    auto audioBlock = juce::dsp::AudioBlock<float> (buffer);
    auto context = juce::dsp::ProcessContextReplacing<float> (audioBlock);
    
    filter.process (context);
}

void EP491SaturationAudioProcessor::boost(juce::AudioBuffer<float>& buffer, float gain, float freq, double sampleRate, int numChannels)
{
    float q = 0.8f;
    
    *iirFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, freq, q, gain);
    
    auto audioBlock = juce::dsp::AudioBlock<float> (buffer);
    auto context = juce::dsp::ProcessContextReplacing<float> (audioBlock);
    
    iirFilter.process (context);
}


juce::AudioProcessorValueTreeState::ParameterLayout EP491SaturationAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Mix params
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "INPUT", 1 }, "Input", juce::NormalisableRange<float> { 0.01f, 1.0f, 0.01f}, 1.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "OUTPUT", 1 }, "Output", juce::NormalisableRange<float> { 0.01f, 1.0f, 0.01f}, 1.0f));

    
    // Layer 1 params
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "DISTTYPE", 1 }, "Distortion Type", juce::StringArray { "Off", "Hard Clip", "Soft Clip", "Fuzz", "Sine", "Diode", "Tanh" }, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "DISTGAIN", 1 }, "Distortion Gain", juce::NormalisableRange<float> { 0.01f, 50.0f, 0.01f, 0.6f }, 1.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "DISTLEVEL", 1 }, "Distortion Level", juce::NormalisableRange<float> { 0.1f, 1.0f, 0.01f }, 1.0f));
    
    
    // Layer 2 params
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "DISTTYPE2", 1 }, "Distortion Type 2", juce::StringArray { "Off", "Hard Clip", "Soft Clip", "Fuzz", "Sine", "Diode", "Tanh" }, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "DISTGAIN2", 1 }, "Distortion Gain 2", juce::NormalisableRange<float> { 0.01f, 50.0f, 0.01f, 0.6f }, 1.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "DISTLEVEL2", 1 }, "Distortion Level 2", juce::NormalisableRange<float> { 0.1f, 1.0f, 0.01f }, 1.0f));
    
    
    // Filter params
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "FILTERTYPE", 1 }, "Filter Type", juce::StringArray { "Lowpass", "Bandpass", "Highpass" }, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "FILTERFREQ", 1}, "Filter Freq", juce::NormalisableRange<float> { 20.0f, 20000.0f, 0.1f, 0.6f }, 20000.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "FILTERRES", 1}, "Filter Resonance", juce::NormalisableRange<float> { 1.0f, 10.0f, 0.01f}, 1.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "FILTERPOS", 1 }, "Filter Position", juce::StringArray { "Pre", "Middle", "Post" }, 0));
    
    
    // Boom params
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "BOOMTYPE", 1 }, "Boom Type", juce::StringArray { "Off", "On" }, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "BOOM", 1 }, "Boom", juce::NormalisableRange<float> { 1.f, 50.0f, 0.01f, 0.6f }, 1.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "BOOMFREQ", 1}, "Boom Freq", juce::NormalisableRange<float> { 20.0f, 200.f, 0.1f, 0.6f }, 200.f));

    
    return { params.begin(), params.end() };
}
