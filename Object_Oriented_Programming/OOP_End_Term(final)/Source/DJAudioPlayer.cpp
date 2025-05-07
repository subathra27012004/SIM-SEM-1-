// The cpp files containus the implementation for loading, playing, stopping, looping, and controlling audio tracks.
/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 28 Jan 2025 4:46:56pm
    Author:  Subathra

  ==============================================================================
*/

// Include the header file
#include "DJAudioPlayer.h"

// Constructor for the class
DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
    : formatManager(_formatManager)
{

}

// Destructor
DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Transport source for track 
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    // Resample source for track 
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Fill the buffer with the following audio block from the resample source
    resampleSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources()
{
    // Release transport source resources
    transportSource.releaseResources();

    // Release resample source resources
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    // Reader for the given URL
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    
    // Check if the file was loaded successfully
    if (reader != nullptr)
    {
        // Create new audioformatereadersource from the reader
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        
        // Set new source for transportsource with the reader sample rate
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
       
        // reset readersource to take ownership of the new source
        readerSource.reset(newSource.release());
    }
}

void DJAudioPlayer::setGain(double gain)
{
    // Check if the gain value is within the range
    if (gain < 0 || gain > 1.0)
    {
        // Console statement if the range is out
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    }
    else {
        // Set the gain for the transport source
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    // Ensure the ratio is within a safe range
    if (ratio < 0.1 || ratio > 100.0)
    {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0.1 and 100");
        return; // Exit the function to prevent crashing
    }

    // Set resample ratio for track speed adjustment
    resampleSource.setResamplingRatio(ratio);
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    // Move the playhead to the specific position
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPostionRelative(double pos)
{
    // Convert position value into absoulute time in seconds
    double posInSecs = transportSource.getLengthInSeconds() * pos;
    
    // set the playback position to the calculated absolute time
    setPosition(posInSecs);
}

void DJAudioPlayer::start()
{
    // Start track from the current position
    transportSource.start();
}
void DJAudioPlayer::stop()
{
    // stop track
    transportSource.stop();
}

void DJAudioPlayer::loop(bool loop)
{
    // loop state in the transport source
    transportSource.setLooping(loop);

    // Restart the track if the loop is on
    if (loop && getPositionRelative() >= 1.0)
    {
        // Restart from beginning
        setPostionRelative(0.0);

        // Start the track
        start();
    }
}

double DJAudioPlayer::getPositionRelative()
{
    // return the current position as a fraction of the total track length
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}