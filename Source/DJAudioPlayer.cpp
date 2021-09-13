/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 14 Aug 2021 9:47:37pm
    Author:  Shivam Maheshwari

  ==============================================================================
*/

#include "DJAudioPlayer.h"


DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) : formatManager(_formatManager) {
    
}

DJAudioPlayer::~DJAudioPlayer() {
    
}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
    
    
    
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
}
void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) {
    
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources() {
    
    transportSource.releaseResources();
    resampleSource.releaseResources();

}

// load track file
void DJAudioPlayer::loadURL(juce::URL audioURL) {
    
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if(reader != nullptr) {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource (new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
       
    }
    
}

// set the gain of the track
void DJAudioPlayer::setGain(double gain) {
    
    if (gain < 0 && gain > 1.0) {
        std::cout << "DJAudioPlayer::setGain should be between 0 and 1" << std::endl;
    } else {
        transportSource.setGain(gain);
    }
}


// set the speed of the track
void DJAudioPlayer::setSpeed(double ratio) {
    
    if (ratio < 0 && ratio > 100.0) {
        std::cout << "DJAudioPlayer::setGain should be between 0 and 1" << std::endl;
    } else {
        resampleSource.setResamplingRatio(ratio);
    }
}

// set the position of the track
void DJAudioPlayer::setPosition(double posInSecs) {
    transportSource.setPosition(posInSecs);
}

// set the relative position of the playhead
void DJAudioPlayer::setPositionRelative(double pos) {
    
    if (pos < 0 && pos > 1.0) {
        std::cout << "DJAudioPlayer::setGain should be between 0 and 1" << std::endl;
    } else {
        double posInSeconds = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSeconds);
    }
}

// start the track
void DJAudioPlayer::start() {
    
    transportSource.start();
}

// stop the track
void DJAudioPlayer::stop() {
    
    transportSource.stop();
}

// get the relative position of the playhead
double DJAudioPlayer::getPositionRelative() {
   if (transportSource.getLengthInSeconds() > 0) {
        return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
    }
    return 1.0;
}
