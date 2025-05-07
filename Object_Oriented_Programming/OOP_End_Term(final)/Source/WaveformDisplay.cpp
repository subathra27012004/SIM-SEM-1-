// The header file contains the implementations which display the waveform of the audio file, manage the loading of audio files, and updating the display based on the playhead position.
/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 30 Jan 2025 1:31:02pm
    Author:  Subathra

  ==============================================================================
*/

// Include all the libraries and the header file
#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
// Constructor to set up the WaveformDisplay object's color, format manager, and cache
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse, juce::Colour color) :
    audioThumb(1000, formatManagerToUse, cacheToUse),
    waveformColour(color),
    fileLoaded(false),
    position(0)
{
    // Add as a listener to audiothumbnail changes
    audioThumb.addChangeListener(this);
}

// Destructor
WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    // Background fill
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Set boarder color to be grey
    g.setColour(juce::Colours::grey);

    // Draw a border
    g.drawRect(getLocalBounds(), 2);

    // Set the waveform color
    g.setColour(waveformColour);

    // If file is loaded
    if (fileLoaded)
    {
        // Draw the waveform using the audiothumbnail object
        audioThumb.drawChannel(g,
            getLocalBounds(),
            0,
            audioThumb.getTotalLength(),
            0,
            1.0f
        );

        // Set playhead color to be light green
        g.setColour(juce::Colours::lightgreen);

        // Draw a very thin rectangle for the playhead
        g.drawRect(position * getWidth(), 0, getWidth() / 200, getHeight());
       
        // fill the playhead
        g.fillRect(position * getWidth(), 0, getWidth() / 200, getHeight());
    }
    else {
        // Font sizr to 20
        g.setFont(juce::FontOptions(20.0f));

        // Message to display in the center
        g.drawText("File not loaded...", getLocalBounds(), juce::Justification::centred, true);
    }
}

void WaveformDisplay::resized()
{

}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    // Clear existing waveform data
    audioThumb.clear();

    // Set the source to new URL
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
}


void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    // Update the UI when changes happens
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    // If the position change
    if (pos != position)
    {
        // update the position of the playhead
        position = pos;

        // Update the UI
        repaint();
    }
}