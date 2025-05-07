// The header file contains the functions for loading, playing, stopping, looping, and controlling audio tracks.
/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 28 Jan 2025 4:46:56pm
    Author:  Subathra

  ==============================================================================
*/
// Include the juce library
#pragma once

#include <JuceHeader.h>

// Include the JUCE class 
class DJAudioPlayer : public juce::AudioSource {
    public:
        // Constructor which takes reference to audioformatmanager
        DJAudioPlayer(juce::AudioFormatManager& _formatManager);
        
        // Cleans up everything when the program closes
        ~DJAudioPlayer();

        // Prepare the audio player before playback
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        
        // Add the next audio block to the audio buffer
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        
        // Release resources
        void releaseResources() override;

        // Load track into the deck using URL
        void loadURL(juce::URL audioURL);

        // Set the gain of the player
        void setGain(double gain);

        // Set the speed of the track
        void setSpeed(double ratio);

        // Set the track position in seconds
        void setPosition(double posInSecs);

        // Set the track position relative
        void setPostionRelative(double pos);

        // Start the track
        void start();

        // Stop the track
        void stop();

        // Loop the track
        void loop(bool loop);

        // Get the relative position of the playhead
        double getPositionRelative();

    private:
        // Reference to audio format manager
        juce::AudioFormatManager& formatManager;
        
        // Pointer to audio source
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        
        // Manages audio playback
        juce::AudioTransportSource transportSource;

        // Resampling source for changing speed
        juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
