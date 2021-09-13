/*
  ==============================================================================

    PlaylistComponent.h
    Created: 16 Aug 2021 10:00:52pm
    Author:  Shivam Maheshwari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"
#include "Track.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component, public juce::TableListBoxModel, public juce::Button::Listener, public juce::TextEditor::Listener {
public:
    /** PlaylistCompnent constructor  */
    PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, juce::AudioFormatManager& formatManagerToUse);

    /** PlaylistComponent destructor */
    ~PlaylistComponent() override;

    /** called when component need to be painted  */
    void paint (juce::Graphics&) override;
    
    /** called every time when window is resized */
    void resized() override;
    
    /** returns the number of rows currently in the table */
    int getNumRows() override;
    
    /** draws background behind one of the row */
    void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    
    /** draws the content inside the cell */
    void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    
    
    /** custom component to render the cell in a row */
    juce::Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected,
                                              juce::Component* existingComponentToUpdate) override;
    
    
    /** called when button is clicked */
    void buttonClicked(juce::Button* button) override;
    
    /** listens on text change in the searchbox and filter tracks from tracklist on the basis of the search query entered */
    void textEditorTextChanged(TextEditor& editor) override;
    
    /** continue playing a track in the given deck */
    void loopTrack(bool pos);
    
    /** play next track in the tracklist */
    void playNextTrack(bool pos);
    
    /** play previous track in the tracklist */
    void playPreviousTrack(bool pos);


private:
    
    /** Table list component */
    juce::TableListBox tableComponent;
    
    /** deckGUI pointers */
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    
    /** Text editor for searchbar */
    juce::TextEditor searchBar;
    
    /** Text button for Add a track button */
    juce::TextButton addButton{"Add a Track"};
    
    /** audio format manager reference */
    juce::AudioFormatManager& formatManager;
    
    /** track list  to store tracks */
    std::vector<Track> trackList;
    
    /** tokenize function to split string into the vector of strings using a separator */
    std::vector<std::string> tokenise(std::string input, char separator);
    
    /** add the track to the tracklist and the csv file for music library data persistance */
    void addTrackToPlaylist();

    /** add tracks to the csv */
    void addToCSV();
    
    /** load tracks from the csv */
    void loadFromCSV();
    
    /** update table component function */
    void updateTableComponent();
    
    /** current index of the tracklist for next and previous track functionality */
    int currentIndex = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
