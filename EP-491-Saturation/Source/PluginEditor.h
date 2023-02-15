/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class EP491SaturationAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    EP491SaturationAudioProcessorEditor (EP491SaturationAudioProcessor&);
    ~EP491SaturationAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    EP491SaturationAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EP491SaturationAudioProcessorEditor)
};
