/*
  ==============================================================================

    DeckGUI.cpp
    Created: 15 Aug 2021 2:12:10pm
    Author:  Shivam Maheshwari

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"


//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse): player(_player), waveformDisplay(formatManagerToUse, cacheToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(posLabel);
    
    addAndMakeVisible(waveformDisplay);
    
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    
    Image repeatImage = juce::ImageCache::getFromMemory(BinaryData::stop_png, BinaryData::stop_pngSize);
    mImageComponent.setImages(true, true, true,
               repeatImage, 0, Colour(255,255,255),
               repeatImage, 0, Colour(255,0,0),
               repeatImage, 0, Colour(100,0,0));
    
    mImageComponent.addListener(this);
    
    posSlider.addListener(this);
    speedSlider.addListener(this);
    volSlider.addListener(this);
    addAndMakeVisible(mImageComponent);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.1, 100.0);
    posSlider.setRange(0.0, 1.0);
    
    volSlider.setValue(0.8);
    speedSlider.setValue(1.0);
    posSlider.setValue(0.0);
    
    startTimer(500);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
    
}

void DeckGUI::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    
    
    // styling the sliders
    volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    posSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    
    volSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    speedSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    posSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    
    // look and feel of the sliders
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::orange);
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colours::skyblue);
    getLookAndFeel().setColour(Slider::trackColourId, Colours::skyblue);
    
//    volSlider.setLookAndFeel(&sliderLookAndFeel);
//    speedSlider.setLookAndFeel(&sliderLookAndFeel);
    posSlider.setLookAndFeel(&sliderLookAndFeel);
    
    // styling the labels
    volLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.setJustificationType(juce::Justification::centred);
    volLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    

    speedLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centred);
    speedLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    

    posLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    posLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    posLabel.setText("Position", juce::dontSendNotification);
    posLabel.setJustificationType(juce::Justification::centred);
    
    // set colour for buttons
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::wheat);
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::wheat);
    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colours::wheat);

    // set colour for button text
    playButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    stopButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    loadButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    
}

void DeckGUI::resized()
{
    
    double rowH = getHeight() / 7;

    
    // set bounds for labels
    volLabel.setBounds(0, 0, getWidth() / 3, rowH);
    speedLabel.setBounds(getWidth() / 3, 0, getWidth() / 3, rowH);
    posLabel.setBounds(2 * getWidth() / 3, 0, getWidth() / 3, rowH);
    
    volSlider.setBounds(0, rowH, getWidth() / 3, rowH * 3);
    speedSlider.setBounds(getWidth() / 3, rowH, getWidth() / 3, rowH * 3);
    posSlider.setBounds(2 * getWidth() / 3, rowH, getWidth() / 3, rowH * 3);
    
    waveformDisplay.setBounds(0, rowH * 4, getWidth(), rowH * 2);
    
    mImageComponent.setBounds(10, rowH * 6 + 10, -10 + getWidth() / 3, 0.7 * rowH);
    stopButton.setBounds(10 + getWidth() / 3, rowH * 6 + 10, -10 + getWidth() / 3, 0.7 * rowH);
    loadButton.setBounds(10 + 2 * getWidth() / 3, rowH * 6 + 10, -20 + getWidth() / 3, 0.7 * rowH);

}

// called when button is clicked
void DeckGUI::buttonClicked(juce::Button* button) {
   
    // handling button clicks for start, stop and load button
    if(button == &mImageComponent) {
        std::cout << "Play button is clicked!" << std::endl;
        player->start();

    } else if (button == &stopButton) {
        std::cout << "Stop button is clicked" << std::endl;
        player->stop();

    } else if (button == &loadButton) {
        juce::FileChooser chooser{"Select a file..."};

        if(chooser.browseForFileToOpen()) {
            player->loadURL(juce::URL{chooser.getResult()});
            waveformDisplay.loadURL(juce::URL{chooser.getResult()});
        }
    }
}


// called when slider value is changed
void DeckGUI::sliderValueChanged(juce::Slider *slider) {
    
    //  for increasing and decreasing the volume
    if(slider == &volSlider) {
        player->setGain(slider->getValue());
    }


    // for increasing and decreasing the speed of the audio file being played
    if(slider == &speedSlider) {
        std::cout << slider->getValue() << " value" << std::endl;
        player->setSpeed(slider->getValue());
    }


    // for changing the position of the audio file being played
    if(slider == &posSlider) {
        player->setPositionRelative(slider->getValue());
    }
}



// called when file is dragged
bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files) {
    
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    
    return true;
}


// called when file is dropped
void DeckGUI::filesDropped(const juce::StringArray &files, int x, int y) {
    
    if(files.size() == 1) {
        player->loadURL(juce::URL{juce::File{files[0]}});
        waveformDisplay.loadURL(juce::URL{juce::File{files[0]}});
    }
    
    std::cout << "DeckGUI::filesDropped" << std::endl;
    
}



void DeckGUI::timerCallback() {
    
    double pos = player->getPositionRelative();
    
    if (pos > 0) {
        waveformDisplay.setPositionRelative(pos);
    }
    
    if (pos >= 1 && loop) {
        player->setPosition(0);
        waveformDisplay.setPositionRelative(0);
        player->start();
    }
    
//    waveformDisplay.setPositionRelative(player->getPositionRelative());
}


void DeckGUI::setLoop() {
    if(loop) {
        loop = false;
    } else {
        loop = true;
    }
}

// load track in both audio player and waveform
void DeckGUI::loadTrack(juce::URL audioURL) {
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}

