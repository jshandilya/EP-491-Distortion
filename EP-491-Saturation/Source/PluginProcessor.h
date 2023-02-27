/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class EP491SaturationAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    EP491SaturationAudioProcessor();
    ~EP491SaturationAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
        
    void setDistortionType(const int choice, juce::AudioBuffer<float>& buffer, float gain, float level, int numChannels);
    
    void hardClip(juce::AudioBuffer<float>& buffer, float gain, float level, int numChannels);
    void softClip(juce::AudioBuffer<float>& buffer, float gain, float level, int numChannels);
    void fuzz(juce::AudioBuffer<float>& buffer, float gain, float level, int numChannels);
    void sine(juce::AudioBuffer<float>& buffer, float gain, float level, int numChannels);
    void bitcrush(juce::AudioBuffer<float>& buffer, float gain, float level, int N, int numChannels);
    void diode(juce::AudioBuffer<float>& buffer, float freq, float gain, float level, double sampleRate, int numChannels);
    void distortionOff(juce::AudioBuffer<float>& buffer, float gain, float level, int numChannels);
    
    juce::AudioProcessorValueTreeState apvts;
    
    // Filter
    
    enum class FilterType
    {
        LowPass,
        BandPass,
        HighPass
    };
    
    void reset() override;
    void setType(int choice);
    
    juce::dsp::StateVariableTPTFilter<float> filter;
    FilterType filterType { FilterType::LowPass };
    
private:
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EP491SaturationAudioProcessor)
};
