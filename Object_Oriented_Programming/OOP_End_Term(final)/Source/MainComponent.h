// The header file contains the main part of the DJ app which combines the 2 audio players, their controls, a playlist, and a mixer to handle playback and user interaction.
// Include all the libraries and the header files
#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "WaveformDisplay.h"

//==============================================================================
// Include all JUCE classes 
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    // Called to set up everything when the program starts
    MainComponent();

    // Cleans up everything when the program closes
    ~MainComponent() override;

    //==============================================================================
    // Prepares the audio system for playback
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    
    // Continuously provides the next piece of audio to play
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    
    // Free up memory when audio stops
    void releaseResources() override;

    //==============================================================================
    // Paint function to draw on the screen
    void paint (juce::Graphics& g) override;
    
    // To resize the components on the screen
    void resized() override;


private:

    // To load and play different types of audio files
    juce::AudioFormatManager formatManager;

    // Stores the waveform previews of audio files
    juce::AudioThumbnailCache thumbCache{100};

    // 1st audio player that plays songs
    DJAudioPlayer player1{ formatManager };

    // Deck 1 controls
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache, juce::Colours::deepskyblue };
    
    // 2nd audio player that plays songs
    DJAudioPlayer player2{formatManager}; 
    
    // Deck 2 controls
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache , juce::Colours::red };

    // Mixes both deck audio together
    juce::MixerAudioSource mixerSource;

    // Manage and show the song list that was loaded into the playlist table
    PlaylistComponent playlistComponent;

    // Prevents copying this class to avoid memory issues
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
