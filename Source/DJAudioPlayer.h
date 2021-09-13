/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 14 Aug 2021 9:47:37pm
    Author:  Shivam Maheshwari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

class DJAudioPlayer : public juce::AudioSource {
public:
    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    /** load track file */
    void loadURL(juce::URL audioURL);
    
    /** set the gain of the track */
    void setGain(double gain);
    
    /** set the speed of the track */
    void setSpeed(double ratio);
    
    /** set the position of the track */
    void setPosition(double posInSecs);
    
    /** set the relative position of the playhead */
    void setPositionRelative(double pos);
    
    /** start the track */
    void start();
    /** stop the track */
    void stop();
    
    /** get the relative position of the playhead */
    double getPositionRelative();
    
    
private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    ResamplingAudioSource resampleSource{&transportSource, false, 2};
}; 
