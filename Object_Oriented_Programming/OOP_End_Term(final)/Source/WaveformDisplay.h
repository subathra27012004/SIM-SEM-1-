// The header file contains the functions which display the waveform of the audio file, manage the loading of audio files, and updating the display based on the playhead position.
/*
  ==============================================================================

    WaveformDisplay.h
    Created: 30 Jan 2025 1:31:02pm
    Author:  Subathra

  ==============================================================================
*/
// Include all the libraries and the header files 
#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
// Include all JUCE classes 
class WaveformDisplay  : public juce::Component,
                         public juce::ChangeListener
{
public:
    // Constructor
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse,
                    juce::Colour color);

    // Cleans up everything when the program closes
    ~WaveformDisplay() override;

    // Paint function to draw on the screen
    void paint (juce::Graphics&) override;

    // To resize the components on the screen
    void resized() override;

    // Called when audio player state changes
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    // Loads the audio file from a URL
    void loadURL(juce::URL audioURL);

    // Set the position of the playhead in the waveform display
    void setPositionRelative(double pos);

private:
    // Audiothumbnail object to render waveform
    juce::AudioThumbnail audioThumb;

    // Color of the waveform
    juce::Colour waveformColour;

    // Variable to check if an audio file is loaded
    bool fileLoaded;

    // Variable to store current position of the playhead
    double position;

    // Prevents copying this class to avoid memory issues
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
