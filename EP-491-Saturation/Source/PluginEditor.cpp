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
    
    setMixSliderWithLabel (inputSlider, inputLabel, audioProcessor.apvts, "INPUT", inputAttachment);
    setMixSliderWithLabel (outputSlider, outputLabel, audioProcessor.apvts, "OUTPUT", outputAttachment);
        
    setSliderWithLabel (gain1Slider, gain1Label, audioProcessor.apvts, "DISTGAIN", gain1Attachment);
    setSliderWithLabel (gain2Slider, gain2Label, audioProcessor.apvts, "DISTGAIN2", gain2Attachment);
    
    setSliderWithLabel (level1Slider, level1Label, audioProcessor.apvts, "DISTLEVEL", level1Attachment);
    setSliderWithLabel (level2Slider, level2Label, audioProcessor.apvts, "DISTLEVEL2", level2Attachment);
    
    setFreqSliderWithLabel (cutoffSlider, cutoffLabel, audioProcessor.apvts, "FILTERFREQ", cutoffAttachment);
    setSliderWithLabel (resSlider, resLabel, audioProcessor.apvts, "FILTERRES", resAttachment);
    
    setSliderWithLabel (boomSlider, boomLabel, audioProcessor.apvts, "BOOM", boomAttachment);
    setFreqSliderWithLabel (boomFreqSlider, boomFreqLabel, audioProcessor.apvts, "BOOMFREQ", boomFreqAttachment);
    
    setSize (650, 500);
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
    const auto boxWidth = 90;
    const auto boxHeight = 30;
    
    const auto sliderWidth = 100;
    const auto sliderHeight = 100;
    
    const auto mixSliderWidth = 100;
    const auto mixSliderHeight = 300;
    
    const auto labelYOffset = 20;
    const auto labelHeight = 20;
    
    inputSlider.setBounds (0, 100, mixSliderWidth, mixSliderHeight);
    inputLabel.setBounds (inputSlider.getX(), inputSlider.getY() - labelYOffset, inputSlider.getWidth(), labelHeight);
    
    outputSlider.setBounds (550, 100, mixSliderWidth, mixSliderHeight);
    outputLabel.setBounds (outputSlider.getX(), outputSlider.getY() - labelYOffset, outputSlider.getWidth(), labelHeight);
    
    distType1Box.setBounds (110, 40, boxWidth * 2, boxHeight);
    distType1Label.setBounds (distType1Box.getX(), distType1Box.getY() - labelYOffset - 10, boxWidth, boxHeight);

    gain1Slider.setBounds (100, 110, sliderWidth, sliderHeight);
    gain1Label.setBounds (gain1Slider.getX(), gain1Slider.getY() - labelYOffset, gain1Slider.getWidth(), labelHeight);

    level1Slider.setBounds (200, 110, sliderWidth, sliderHeight);
    level1Label.setBounds (level1Slider.getX(), level1Slider.getY() - labelYOffset, level1Slider.getWidth(), labelHeight);

    distType2Box.setBounds (340, 40, boxWidth * 2, boxHeight);
    distType2Label.setBounds (distType2Box.getX(), distType2Box.getY() - labelYOffset - 10, boxWidth, boxHeight);

    gain2Slider.setBounds (330, 110, sliderWidth, sliderHeight);
    gain2Label.setBounds (gain2Slider.getX(), gain2Slider.getY() - labelYOffset, gain2Slider.getWidth(), labelHeight);

    level2Slider.setBounds (430, 110, sliderWidth, sliderHeight);
    level2Label.setBounds (level2Slider.getX(), level2Slider.getY() - labelYOffset, level2Slider.getWidth(), labelHeight);

    filterTypeBox.setBounds (340, 260, boxWidth, boxHeight);
    filterTypeLabel.setBounds (filterTypeBox.getX(), filterTypeBox.getY() - labelYOffset - 10, boxWidth, boxHeight);

    filterPosBox.setBounds (filterTypeBox.getX() + boxWidth, 260, boxWidth, boxHeight);
    filterPosLabel.setBounds (filterPosBox.getX(), filterPosBox.getY() - labelYOffset - 10, boxWidth, boxHeight);

    cutoffSlider.setBounds (330, 330, sliderWidth, sliderHeight);
    cutoffLabel.setBounds (cutoffSlider.getX(), cutoffSlider.getY() - labelYOffset, cutoffSlider.getWidth(), labelHeight);

    resSlider.setBounds (430, 330, sliderWidth, sliderHeight);
    resLabel.setBounds (resSlider.getX(), resSlider.getY() - labelYOffset, resSlider.getWidth(), labelHeight);
    
    boomBox.setBounds (110, 260, boxWidth * 2, boxHeight);
    boomBoxLabel.setBounds (boomBox.getX(), boomBox.getY() - labelYOffset - 10, boxWidth, boxHeight);
    
    
    boomFreqSlider.setBounds (100, 330, sliderWidth, sliderHeight);
    boomFreqLabel.setBounds (boomFreqSlider.getX(), boomFreqSlider.getY() - labelYOffset, boomFreqSlider.getWidth(), labelHeight);
    
    boomSlider.setBounds (200, 330, sliderWidth, sliderHeight);
    boomLabel.setBounds (boomSlider.getX(), boomSlider.getY() - labelYOffset, boomSlider.getWidth(), labelHeight);
    
    
    
    
}

using BoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

void EP491SaturationAudioProcessorEditor::setComboBoxes()
{
    juce::StringArray distChoices { "Off", "Hard Clip", "Soft Clip", "Fuzz", "Sine", "Diode", "Tanh" };
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
    
    juce::StringArray boomChoices { "Off", "On"};
    boomBox.addItemList (boomChoices, 1);
    addAndMakeVisible (boomBox);
    
    boomBoxAttachment = std::make_unique<BoxAttachment>(audioProcessor.apvts, "BOOMTYPE", boomBox);
    
    boomBoxLabel.setColour (juce::Label::ColourIds::textColourId, juce::Colours::white);
    boomBoxLabel.setFont (15.0f);
    boomBoxLabel.setJustificationType (juce::Justification::left);
    addAndMakeVisible (boomBoxLabel);
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

void EP491SaturationAudioProcessorEditor::setMixSliderWithLabel (juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<SliderAttachment>& attachment)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    addAndMakeVisible(slider);
    
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
    
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    
    slider.setTextValueSuffix (" dB");
    
    addAndMakeVisible(label);
}

void EP491SaturationAudioProcessorEditor::setFreqSliderWithLabel (juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<SliderAttachment>& attachment)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    addAndMakeVisible(slider);
    
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
    
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    
    slider.setTextValueSuffix (" Hz");
    
    addAndMakeVisible(label);
}
