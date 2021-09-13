/*
  ==============================================================================

    ControlDeck.cpp
    Created: 30 Aug 2021 3:20:41pm
    Author:  Shivam Maheshwari

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ControlDeck.h"

//==============================================================================
ControlDeck::ControlDeck(DJAudioPlayer* _player, PlaylistComponent& playlistComponent, bool _pos) : player(_player),
playlistComp(playlistComponent), pos(_pos)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    addAndMakeVisible(nextButton);
    addAndMakeVisible(previousButton);
    addAndMakeVisible(loopButton);
    
    nextButton.addListener(this);
    previousButton.addListener(this);
    loopButton.addListener(this);

}

ControlDeck::~ControlDeck()
{
}

void ControlDeck::paint (juce::Graphics& g)
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
//    g.drawText ("ControlDeck", getLocalBounds(),
//                juce::Justification::centred, true);   // draw some placeholder text
//
    
    nextButton.setColour(juce::TextButton::buttonColourId, juce::Colours::wheat);
    previousButton.setColour(juce::TextButton::buttonColourId, juce::Colours::wheat);
    loopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::wheat);
    
    nextButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    previousButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    loopButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    
}

void ControlDeck::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    double rowH = getHeight() / 2;
        
    nextButton.setBounds(10, rowH, -10 + getWidth() / 3, rowH);
    previousButton.setBounds(10 + getWidth() / 3, rowH, -10 + getWidth() / 3, rowH);
    loopButton.setBounds(10 + 2 * getWidth() / 3, rowH, -20 + getWidth() / 3, rowH);

}


// called when button is clicked
void ControlDeck::buttonClicked(juce::Button* button) {
   
    // handling button clicks for start, stop and load button
     if(button == &nextButton) {
         
         playlistComp.playNextTrack(pos);
         player->start();

     } else if (button == &previousButton) {
         playlistComp.playPreviousTrack(pos);
         player->start();
     } else if (button == &loopButton) {
         playlistComp.loopTrack(pos);
         
     }
}


