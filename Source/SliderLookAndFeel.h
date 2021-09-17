/*
  ==============================================================================

    SliderLookAndFeel.h
    Created: 17 Sep 2021 7:28:13pm
    Author:  Shivam Maheshwari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SliderLookAndFeel  : public juce::Component, public juce::LookAndFeel_V4
{
public:
    SliderLookAndFeel();
    ~SliderLookAndFeel() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional,
                          float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderLookAndFeel)
};
