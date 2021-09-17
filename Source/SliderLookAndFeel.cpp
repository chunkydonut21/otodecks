/*
  ==============================================================================

    SliderLookAndFeel.cpp
    Created: 17 Sep 2021 7:28:13pm
    Author:  Shivam Maheshwari

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SliderLookAndFeel.h"

//==============================================================================
SliderLookAndFeel::SliderLookAndFeel()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

SliderLookAndFeel::~SliderLookAndFeel()
{
}

void SliderLookAndFeel::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("SliderLookAndFeel", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void SliderLookAndFeel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}


void SliderLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional,
float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) {
    
    double radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
    double centreX = (float) x + (float) width  * 0.5f;
    double centreY = (float) y + (float) height * 0.5f;
    double rx = centreX - radius;
    double ry = centreY - radius;
    double rw = radius * 2.0f;
    double angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    // setting the background colour
    g.setColour(juce::Colours::skyblue);
    g.fillEllipse(rx, ry, rw, rw);


    juce::Path p;
    double pointerLength = radius;
    double pointerThickness = 8.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation (angle).translated (centreX,centreY));
    
    // setting colour for the slider pointer
    g.setColour(juce::Colours::darkgrey);

    g.fillPath(p);

    // setting colour for the ellipse
    g.setColour(juce::Colours::darkgrey);

    double smallw = rw * 0.45;
    g.fillEllipse(centreX -(smallw /2), centreY - (smallw/2), smallw, smallw);

}
