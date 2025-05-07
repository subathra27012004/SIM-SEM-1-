// The header file contains the DJ deck, handling playback, UI and file loading. 
/*
  ==============================================================================

    DeckGUI.h
    Created: 28 Jan 2025 10:53:35pm
    Author:  Subathra

  ==============================================================================
*/

// Include all the libraries and the header files 
#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "RotatingDisk.h"
#include "DrumPadComponent.h"


//==============================================================================
/*
*/
// Include all JUCE classes 
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer
{
public:
    // Constructor which has the deck GUI with player, format manager and cache
    DeckGUI(DJAudioPlayer* _player,
            juce::AudioFormatManager & formatManagerToUse,
            juce::AudioThumbnailCache & cacheToUse,
            juce::Colour color);
    
    // Cleans up everything when the program closes
    ~DeckGUI() override;

    // Paint function to draw on the screen
    void paint (juce::Graphics&) override;
    
    // To resize the components on the screen
    void resized() override;

    // Called when a button is clicked
    void buttonClicked(juce::Button* button) override;

    // Called when slider value changes
    void sliderValueChanged(juce::Slider* slider) override;

    // Checks if files are dragged into the component
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    
    // Called when files are dropped in the component
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    // Update UI at regular intervals
    void timerCallback() override;

    // Loads audio track into the deck
    void loadTrack(const juce::String& filePath);

    // Display the title track below the waveform
    void setCurrentTrackTitle(const juce::String& title);

private:

    // loop variable to keep track if the looping is on
    bool looping = false;

    // Store the current title of the track
    juce::String currentTrackTitle;

    // Button to play the track
    juce::TextButton playButton{ "PLAY" };

    // Button to stop the track
    juce::TextButton stopButton{ "STOP" };

    // Button to loop the track
    juce::TextButton loopButton{ "LOOP" };

    // Button to load the track
    juce::TextButton loadButton{ "LOAD" };

    // Volumn slider to adjust the volumn of the track
    juce::Slider volSlider;

    // Speed slider to adjust the speed of the track
    juce::Slider speedSlider;

    // Position slider to adjust the position of the track
    juce::Slider posSlider;

    // Volumn label to be shown on the slider
    juce::Label volSliderLabel;

    // Speed label to be shown on the slider
    juce::Label speedSliderLabel;

    // Position label to be shown on the slider
    juce::Label posSliderLabel;

    // Pointer to the audio player
    DJAudioPlayer* player;

    // File chooser for loading tracks
    juce::FileChooser fChooser{ "Select a file..." };

    // Custom UI theme
    juce::LookAndFeel_V4 lookandfeel;

    // Waveform of the loaded track
    WaveformDisplay waveformDisplay;

    // Rotating disk effect
    RotatingDisk rotatingDisk;

    // Drum pad for real feature in dj program
    DrumPadComponent DrumPad;

    // Prevents copying this class to avoid memory issues
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
