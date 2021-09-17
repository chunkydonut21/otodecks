/*
  ==============================================================================

    DeckGUI.h
    Created: 15 Aug 2021 2:12:10pm
    Author:  Shivam Maheshwari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "SliderLookAndFeel.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener,
public juce::FileDragAndDropTarget, public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* player, juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    /** called when component need to be painted  */
    void paint (juce::Graphics&) override;
    
    /** called when window is resized */
    void resized() override;
    
    /** called when button is clicked */
    void buttonClicked(juce::Button *) override;
    
    /** called when slider value is changed */
    void sliderValueChanged(juce::Slider *slider) override;
    
    /** called when file is dragged */
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    
    /** called when file is dropped */
    void filesDropped(const juce::StringArray &files, int x, int y) override;
    
    void timerCallback() override;
    
    /** load track in both audio player and waveform display */
    void loadTrack(juce::URL audioURL);
    
    void setLoop();

private:
    
    /** text button for play, stop and load */
    juce::TextButton playButton{"PLAY"};
    juce::TextButton stopButton{"STOP"};
    juce::TextButton loadButton{"LOAD"};

    
    /** slider for volume, speed and position */
    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    
    /** label for speed, volume and position */
    juce::Label speedLabel;
    juce::Label volLabel;
    juce::Label posLabel;
    
    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;
    
    SliderLookAndFeel sliderLookAndFeel;
    
    bool loop;
    
    juce::ImageButton mImageComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
