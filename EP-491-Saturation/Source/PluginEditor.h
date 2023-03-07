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
    using BoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    // Mix
    juce::Slider inputSlider, outputSlider;
    std::unique_ptr<SliderAttachment> inputAttachment, outputAttachment;
    juce::Label inputLabel { "Input", "Input" }, outputLabel { "Output", "Output" };
    
    // Distortion Type
    juce::ComboBox distType1Box, distType2Box;
    std::unique_ptr<BoxAttachment> distType1Attachment, distType2Attachment;
    juce::Label distType1Label { "Distortion Tyoe 1", "Type 1"}, distType2Label { "Distortion Tyoe 2", "Type 2"};
    
    // Gain
    juce::Slider gain1Slider, gain2Slider;
    std::unique_ptr<SliderAttachment> gain1Attachment, gain2Attachment;
    juce::Label gain1Label { "Gain 1", "Gain 1" }, gain2Label { "Gain 2", "Gain 2" };
    
    // Level
    juce::Slider level1Slider, level2Slider;
    std::unique_ptr<SliderAttachment> level1Attachment, level2Attachment;
    juce::Label level1Label { "Level 1", "Level 1" }, level2Label { "Level 2", "Level 2" };
    
    // Filter
    juce::ComboBox filterTypeBox;
    std::unique_ptr<BoxAttachment> filterTypeAttachment;
    juce::Label filterTypeLabel { "Filter Type", "Filter Type" };
    
    juce::Slider cutoffSlider, resSlider;
    std::unique_ptr<SliderAttachment> cutoffAttachment, resAttachment;
    juce::Label cutoffLabel { "Filter Cutoff", "Cutoff" }, resLabel { "Filter Resonance", "Res" };
    
    
    void setComboBoxes();

    void setSliderWithLabel (juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<SliderAttachment>& attachment);
    
    
    EP491SaturationAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EP491SaturationAudioProcessorEditor)
};
