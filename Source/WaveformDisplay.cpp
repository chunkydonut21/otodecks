/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 15 Aug 2021 6:00:06pm
    Author:  Shivam Maheshwari

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse) :
    audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0)
{
    
    audioThumb.addChangeListener(this);

}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::orange);
    
    // check if file is loaded or not
    // if it is loaded, then display the waveform otherwise show the File not loaded message
    if(fileLoaded) {
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
        
        g.setColour(juce::Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
        
    } else {
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(), juce::Justification::centred, true);   // draw some placeholder text
    }

}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}


// load the track
void WaveformDisplay::loadURL(juce::URL audioURL) {
    std::cout << "WaveformDisplay::loadURL" << std::endl;
    
    audioThumb.clear();
    
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    
    if (fileLoaded) {
        std::cout << "WaveformDisplay::loadURL loaded" << std::endl;
        repaint();
    } else {
        std::cout << "WaveformDisplay::loadURL not loaded" << std::endl;
    }
}


// receives event callback
void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source) {
    std::cout << "wfd: change received!" << std::endl;
    repaint();
}

// set the relative position of the playhead
void WaveformDisplay::setPositionRelative(double pos) {
    
    
    if(position != pos) {
        repaint();
    }
    position = pos;
}
