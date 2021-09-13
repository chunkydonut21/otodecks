/*
  ==============================================================================

    WaveformDisplay.h
    Created: 15 Aug 2021 6:00:06pm
    Author:  Shivam Maheshwari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay : public juce::Component, public juce::ChangeListener
{
public:
    /** waveform display constructor */
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse);
    
    /** waveform display destructor */
    ~WaveformDisplay() override;

    /** called when component need to be painted  */
    void paint (juce::Graphics&) override;
    
    /** called every time when window is resized */
    void resized() override;
    
    /** receives event callback */
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
    /** load the track */
    void loadURL(juce::URL audioURL);
    
    /** set the relative position of the playhead */
    void setPositionRelative(double pos);
    

private:
    
    juce::AudioThumbnail audioThumb;
    
    bool fileLoaded;
    
    double position;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
