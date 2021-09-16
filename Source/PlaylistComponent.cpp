/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 16 Aug 2021 10:00:52pm
    Author:  Shivam Maheshwari

  ==============================================================================
*/

#include <JuceHeader.h>

#include <filesystem>
#include <iostream>


#include "PlaylistComponent.h"
#include <fstream>

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, juce::AudioFormatManager& formatManagerToUse): deckGUI1(_deckGUI1), deckGUI2(_deckGUI2), formatManager(formatManagerToUse)
{

    // create columns for the table component
    tableComponent.getHeader().addColumn("Track Title", 1, 450);
    tableComponent.getHeader().addColumn("Load to Deck1", 2, 100);
    tableComponent.getHeader().addColumn("Load to Deck2", 3, 100);
    tableComponent.getHeader().addColumn("Length", 4, 50);
    tableComponent.getHeader().addColumn("Remove", 5, 50);
    
    
    addButton.addListener(this);
    searchBar.addListener(this);
    
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(addButton);
    addAndMakeVisible(searchBar);
    
    // loading tracks from the csv to display the persisted data
    loadFromCSV();

}

PlaylistComponent::~PlaylistComponent()
{

}

void PlaylistComponent::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::darkgrey); // set colour for the rectangle
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white); // set text colour
    g.setFont (14.0f); // set font size
    g.drawText ("PlaylistComponent", getLocalBounds(), juce::Justification::centred, true);   // draw some placeholder text
    
    // set placeholder colour for search bar
    searchBar.setTextToShowWhenEmpty("Search your favourite track...", juce::Colours::darkgrey);
    // set background colour for search bar
    searchBar.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    // set text colour for search bar
    searchBar.setColour(juce::TextEditor::textColourId, juce::Colours::darkgrey);
    
    
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    int rowH = getHeight() / 10;


    // setting bounds for table component, add button and search bar
    tableComponent.setBounds(0, rowH, getWidth(), getHeight());
    addButton.setBounds(4 * getWidth() / 5, 0, getWidth() / 5 , rowH);
    searchBar.setBounds(0, 0, 4 * getWidth()/5, rowH);
    
}


/** returns the number of rows currently in the table */
int PlaylistComponent::getNumRows() {
    return int(trackList.size());
}


/** draws background behind one of the row */
void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
    
    // style the bsckground of the row differently if the row is selected or not
    if(rowIsSelected) {
        g.fillAll(juce::Colours::orange);
        g.setColour(juce::Colours::white);
    } else {
        g.fillAll(juce::Colours::darkgrey);
        g.setColour (juce::Colours::white);
    }
}


/** draws the content inside the cell */
void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    
    if(columnId == 1) {
        g.drawText(trackList[rowNumber].trackName, 2, 1, width - 4, height, juce::Justification::centredLeft, true);
    } else if (columnId == 4) {
        g.drawText(trackList[rowNumber].trackLength, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
}


/** custom component to render the cell in a row */
juce::Component* PlaylistComponent::refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected,
                                          juce::Component* existingComponentToUpdate) {
 
    // load button for left side of the deck
    if(columnId == 2) {
        if(existingComponentToUpdate == nullptr) {
            
            juce::TextButton* btn = new juce::TextButton{"Load"};
            juce::String id{"Left:" + std::to_string(rowNumber)};
            
            btn->setComponentID(id);
            
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    
    
    // load button for the right side of the deck
    if(columnId == 3) {
        if(existingComponentToUpdate == nullptr) {
            
            juce::TextButton* btn = new juce::TextButton{"Load"};
            juce::String id{"Right:" + std::to_string(rowNumber)};
            
            btn->setComponentID(id);
            
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    
    // delete button to remove track from the track list
    if(columnId == 5) {
        if(existingComponentToUpdate == nullptr) {
            
            juce::TextButton* btn = new juce::TextButton{"Delete"};
            juce::String id{std::to_string(rowNumber)};
            
            btn->setComponentID(id);
            
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    
    return existingComponentToUpdate;
}


/** called when button is clicked */
void PlaylistComponent::buttonClicked(juce::Button* button) {
    
    // add track to track list when clicked on the add button
    if (button == &addButton) {
        
        addTrackToPlaylist();
        
    } else {
        
        // if the button clicked is one of the button created via custom component then tokenise the
        // string on the basis of separator.
        std::vector<std::string> data = tokenise(button->getComponentID().toStdString(), ':');
        
        if(data[0] == "Left"){
            int id = std::stoi(data[1]);
            // load music to the deck 1
            deckGUI1->loadTrack(juce::URL{juce::File(trackList[id].path)});
        } else if (data[0] == "Right") {
            int id = std::stoi(data[1]);
            // load music to the deck 2
            deckGUI2->loadTrack(juce::URL{juce::File(trackList[id].path)});
        } else {
            std::cout << "Delete Track: "  << data[0] << std::endl;
            // remove track from the track list if clicked on the delete button
            trackList.erase(trackList.begin() + std::stoi(data[0]));
            // update the csv file
            addToCSV();
            // update the table component
            updateTableComponent();
        }
        
        std::cout << "PlaylistComponent::buttonClicked" << data[0] << " " << data[1] << std::endl;
    }
}

/** add the track to the tracklist and the csv file for music library data persistance */
void PlaylistComponent::addTrackToPlaylist() {
    
    std::cout << "load a file" << std::endl;
    FileChooser chooser{"Select file..."};
    
    if(chooser.browseForFileToOpen()) {
    
        // get the title and file path of the track
        std::string title = chooser.getResult().getFileName().toStdString();
        std::string filePath = chooser.getResult().getFullPathName().toStdString();
           
        // create the format reader
        auto* formatReader = formatManager.createReaderFor(chooser.getResult());
        
        // calculate the track length
        int trackLength = formatReader->lengthInSamples / formatReader->sampleRate;
        
        // convert track length in min:sec format
        std::string minutes = std::to_string(trackLength / 60);
        std::string seconds = std::to_string(trackLength % 60);
        
        std::string total = minutes + ":" + seconds;
                      
        // check if the track already exist in the track list
        for (Track const& item : trackList) {
            if(item.trackName == title) {
                std::cout << "PlaylistComponent::addTrackToPlaylist - File already exists" << std::endl;
                return;
            }
        }
           
        // create track instance
        Track track{title, filePath, total};
        // push the track to the tracklist
        trackList.push_back(track);
        
        // store and persist the track using csv
        addToCSV();
        
        if(formatReader != nullptr) {
            delete formatReader;
        }
        
        // Update the table component when table is updated
        updateTableComponent();
        
    }

}


// tokenize function to split string into the vector of strings using a separator
std::vector<std::string> PlaylistComponent::tokenise(std::string input, char separator) {
    std::string token;
    std::vector<std::string> tokens;

    signed int start, end;

    start = int (input.find_first_not_of(separator, 0));

    do {
        end = int (input.find_first_of(separator, start));

        if (start == input.length() || start == end) {
            break;
        }
        if (end >= 0) {
            token = input.substr(start,end-start);
        }
        else {
            token = input.substr(start, input.length() - start);
        }

        tokens.push_back(token);
        start = end +1;
    } while(end>0);

    return tokens;
}


// listens on text change in the searchbox and filter tracks from tracklist on the basis of the search query entered
void PlaylistComponent::textEditorTextChanged(TextEditor& editor) {
    // if the search text is empty string lthen reset the tracklist
    loadFromCSV();
    
    // get the string entered in the search bar
    std::string query = editor.getText().toStdString();
        
    std::vector<Track> tracks;
    
    // find the name entered in the search query in the tracklist vector
    for (Track const& track : trackList) {
        if (track.trackName.substr(0, query.size()) == query) {
            tracks.push_back(track);
        }
    }
    
    // update the tracklist vector
    trackList = tracks;
    
    // Update the list when the vector is updated
    updateTableComponent();

}


void PlaylistComponent::addToCSV() {
    
    // open the csv file
    std::ofstream out{"data.csv"};
    
    for (Track const& track : trackList) {
        // convert track into the string separated by comma
        std::string line = track.trackName + "," + track.path + "," + track.trackLength;
        
        // append the line in the csv file
        out << line << "\n";
        
    }
    
    // close the csv file
    out.close();

}


// load tracks from the csv
void PlaylistComponent::loadFromCSV() {
    
    std::ifstream csvFile{"data.csv"};
    std::string line;
    
    // vector of track
    std::vector<Track> tracks;

    // if csv file is open
    if (csvFile.is_open()) {
        // read each line in the csv
        while(std::getline(csvFile, line)) {
            try {
                
                // convert string into vector of strings
                std::vector<std::string> data = tokenise(line, ',');
                
                // convert the vector of string into the track object
                Track track{data[0], data[1], data[2]};
                
                // add the track to the tracks vector
                tracks.push_back(track);
                
            } catch(const std::exception& e) {
                std::cout << "PlaylistComponent::loadFromCSV Bad Format" << std::endl;
            }
        }
    }
    
    // assgin the tracks vector to the tracklist
    trackList = tracks;
}




// update table component function
void PlaylistComponent::updateTableComponent() {
    tableComponent.updateContent();
    tableComponent.repaint();
}


void PlaylistComponent::loopTrack(bool pos) {
    if(pos) {
        deckGUI1->setLoop();
    } else {
        deckGUI2->setLoop();
    }
}


void PlaylistComponent::playNextTrack(bool pos) {
    std::cout << "currentIndex" << currentIndex << std::endl;
    
    if(trackList.size() < 1) {
        std::cout << "PlaylistComponent::playNextTrack No track found" << std::endl;
        return;
    }
    
    if(currentIndex < trackList.size()) {
        if(pos) {
            deckGUI1->loadTrack(juce::URL{juce::File(trackList[currentIndex].path)});
        } else {
            deckGUI2->loadTrack(juce::URL{juce::File(trackList[currentIndex].path)});
        }
    } else {
        std::cout << "PlaylistComponent::playNextTrack reached the end of the tracklist " << std::endl;
        currentIndex = 0;
        if(pos) {
            deckGUI1->loadTrack(juce::URL{juce::File(trackList[currentIndex].path)});
        } else {
            deckGUI2->loadTrack(juce::URL{juce::File(trackList[currentIndex].path)});
        }
    }
    
    currentIndex++;
}

void PlaylistComponent::playPreviousTrack(bool pos) {
    if(trackList.size() < 1) {
        std::cout << "PlaylistComponent::playPreviousTrack No track found" << std::endl;
        return;
    }
    
    if(currentIndex <= trackList.size() && currentIndex > 0) {
        if(pos) {
            deckGUI1->loadTrack(juce::URL{juce::File(trackList[currentIndex].path)});
        } else {
            deckGUI2->loadTrack(juce::URL{juce::File(trackList[currentIndex].path)});
        }
    } else {
        std::cout << "PlaylistComponent::playPreviousTrack reached the start of the tracklist" << std::endl;
        currentIndex = int (trackList.size()) - 1;
        if(pos) {
            deckGUI1->loadTrack(juce::URL{juce::File(trackList[currentIndex].path)});
        } else {
            deckGUI2->loadTrack(juce::URL{juce::File(trackList[currentIndex].path)});
        }
    }
    
    currentIndex--;
}
