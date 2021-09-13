/*
  ==============================================================================

    Track.h
    Created: 19 Aug 2021 10:08:14pm
    Author:  Shivam Maheshwari

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <string>



class Track {
public:
    /** Track constructor which takes track name,  path of the track and lefth of the track as a parameter */
    Track(std::string trackName, std::string path, std::string trackLength);
    
    /** track name */
    std::string trackName;
    /** track path */
    std::string path;
    /** track length */
    std::string trackLength;
    
};
