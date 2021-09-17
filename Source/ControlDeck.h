/*
  ==============================================================================

    ControlDeck.h
    Created: 30 Aug 2021 3:20:41pm
    Author:  Shivam Maheshwari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
/*
*/
class ControlDeck  : public juce::Component, public juce::Button::Listener
{
public:
    ControlDeck(DJAudioPlayer* player, PlaylistComponent& playlistComponent, bool position);
    ~ControlDeck() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** called when button is clicked */
    void buttonClicked(juce::Button *) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlDeck)
    
    juce::ImageButton nextButton{"NEXT"};
    juce::ImageButton previousButton{"PREVIOUS"};
    juce::ImageButton loopButton{"LOOP"};
    
    
    DJAudioPlayer* player;
    
    PlaylistComponent& playlistComp;
    
    bool pos;
};
