/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EP491SaturationAudioProcessorEditor::EP491SaturationAudioProcessorEditor (EP491SaturationAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setComboBoxes();
    
    setSliderWithLabel (inputSlider, inputLabel, audioProcessor.apvts, "INPUT", inputAttachment);
    setSliderWithLabel (outputSlider, outputLabel, audioProcessor.apvts, "OUTPUT", outputAttachment);
        
    setSliderWithLabel (gain1Slider, gain1Label, audioProcessor.apvts, "DISTGAIN", gain1Attachment);
    setSliderWithLabel (gain2Slider, gain2Label, audioProcessor.apvts, "DISTGAIN2", gain2Attachment);
    
    setSliderWithLabel (level1Slider, level1Label, audioProcessor.apvts, "DISTLEVEL", level1Attachment);
    setSliderWithLabel (level2Slider, level2Label, audioProcessor.apvts, "DISTLEVEL2", level2Attachment);
    
    setSliderWithLabel (cutoffSlider, cutoffLabel, audioProcessor.apvts, "FILTERFREQ", cutoffAttachment);
    setSliderWithLabel (resSlider, resLabel, audioProcessor.apvts, "FILTERRES", resAttachment);
    
    setSize (600, 400);
}

EP491SaturationAudioProcessorEditor::~EP491SaturationAudioProcessorEditor()
{
}

//==============================================================================
void EP491SaturationAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void EP491SaturationAudioProcessorEditor::resized()
{
    const auto padding = 40;
    
    const auto startX = 10;
    const auto startY = 55;
    
    const auto boxWidth = 90;
    const auto boxHeight = 30;
    
    const auto sliderWidth = 100;
    const auto sliderHeight = 100;
    
    const auto labelYOffset = 20;
    const auto labelHeight = 20;
    
    inputSlider.setBounds (0, getHeight() / 2 - 50, sliderWidth, sliderHeight);
    inputLabel.setBounds (inputSlider.getX(), inputSlider.getY() - labelYOffset, inputSlider.getWidth(), labelHeight);
    
    outputSlider.setBounds (480, getHeight() / 2 - 50, sliderWidth, sliderHeight);
    outputLabel.setBounds (outputSlider.getX(), outputSlider.getY() - labelYOffset, outputSlider.getWidth(), labelHeight);
    
    distType1Box.setBounds (80, 40, boxWidth, boxHeight);
    distType1Label.setBounds (distType1Box.getX(), distType1Box.getY() - labelYOffset - 10, boxWidth, boxHeight);
    
    gain1Slider.setBounds (80, 110, sliderWidth, sliderHeight);
    gain1Label.setBounds (gain1Slider.getX(), gain1Slider.getY() - labelYOffset, gain1Slider.getWidth(), labelHeight);
    
    level1Slider.setBounds (80, 270, sliderWidth, sliderHeight);
    level1Label.setBounds (level1Slider.getX(), level1Slider.getY() - labelYOffset, level1Slider.getWidth(), labelHeight);
    
    distType2Box.setBounds (240, 40, boxWidth, boxHeight);
    distType2Label.setBounds (distType2Box.getX(), distType2Box.getY() - labelYOffset - 10, boxWidth, boxHeight);
    
    gain2Slider.setBounds (240, 110, sliderWidth, sliderHeight);
    gain2Label.setBounds (gain2Slider.getX(), gain2Slider.getY() - labelYOffset, gain2Slider.getWidth(), labelHeight);
    
    level2Slider.setBounds (240, 270, sliderWidth, sliderHeight);
    level2Label.setBounds (level2Slider.getX(), level2Slider.getY() - labelYOffset, level2Slider.getWidth(), labelHeight);
    
    filterTypeBox.setBounds (400, 40, boxWidth, boxHeight);
    filterTypeLabel.setBounds (filterTypeBox.getX(), filterTypeBox.getY() - labelYOffset - 10, boxWidth, boxHeight);
    
    filterPosBox.setBounds (filterTypeBox.getX() + boxWidth + 10, 40, boxWidth, boxHeight);
    filterPosLabel.setBounds (filterPosBox.getX(), filterPosBox.getY() - labelYOffset - 10, boxWidth, boxHeight);
    
    cutoffSlider.setBounds (400, 110, sliderWidth, sliderHeight);
    cutoffLabel.setBounds (cutoffSlider.getX(), cutoffSlider.getY() - labelYOffset, cutoffSlider.getWidth(), labelHeight);
    
    resSlider.setBounds (400, 270, sliderWidth, sliderHeight);
    resLabel.setBounds (resSlider.getX(), resSlider.getY() - labelYOffset, resSlider.getWidth(), labelHeight);
}

using BoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

void EP491SaturationAudioProcessorEditor::setComboBoxes()
{
    juce::StringArray distChoices { "Off", "Hard Clip", "Soft Clip", "Fuzz", "Sine", "Diode" };
    distType1Box.addItemList (distChoices, 1);
    addAndMakeVisible (distType1Box);
    distType2Box.addItemList (distChoices, 1);
    addAndMakeVisible (distType2Box);
    
    distType1Attachment = std::make_unique<BoxAttachment>(audioProcessor.apvts, "DISTTYPE", distType1Box);
    distType2Attachment = std::make_unique<BoxAttachment>(audioProcessor.apvts, "DISTTYPE2", distType2Box);
    
    distType1Label.setColour (juce::Label::ColourIds::textColourId, juce::Colours::white);
    distType1Label.setFont (15.0f);
    distType1Label.setJustificationType (juce::Justification::left);
    addAndMakeVisible (distType1Label);
    
    distType2Label.setColour (juce::Label::ColourIds::textColourId, juce::Colours::white);
    distType2Label.setFont (15.0f);
    distType2Label.setJustificationType (juce::Justification::left);
    addAndMakeVisible (distType2Label);
    
    juce::StringArray filterChoices {  "Lowpass", "Bandpass", "Highpass" };
    filterTypeBox.addItemList (filterChoices, 1);
    addAndMakeVisible (filterTypeBox);
    
    filterTypeAttachment = std::make_unique<BoxAttachment>(audioProcessor.apvts, "FILTERTYPE", filterTypeBox);
    
    filterTypeLabel.setColour (juce::Label::ColourIds::textColourId, juce::Colours::white);
    filterTypeLabel.setFont (15.0f);
    filterTypeLabel.setJustificationType (juce::Justification::left);
    addAndMakeVisible (filterTypeLabel);
    
    juce::StringArray filterPosChoices { "Pre", "Middle", "Post" };
    filterPosBox.addItemList (filterPosChoices, 1);
    addAndMakeVisible (filterPosBox);
    
    filterPosAttachment = std::make_unique<BoxAttachment>(audioProcessor.apvts, "FILTERPOS", filterPosBox);
    
    filterPosLabel.setColour (juce::Label::ColourIds::textColourId, juce::Colours::white);
    filterPosLabel.setFont (15.0f);
    filterPosLabel.setJustificationType (juce::Justification::left);
    addAndMakeVisible (filterPosLabel);
}

void EP491SaturationAudioProcessorEditor::setSliderWithLabel (juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<SliderAttachment>& attachment)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 25);
    addAndMakeVisible(slider);
    
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
    
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}
