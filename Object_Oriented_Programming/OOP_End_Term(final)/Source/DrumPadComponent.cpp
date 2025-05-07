// The cpp file contains the implrmentation of the management of the drum pads with audio playback, volume control, and looping functionality.
/*
  ==============================================================================

    DrumPadComponent.cpp
    Created: 24 Feb 2025 11:07:32pm
    Author:  Subathra

  ==============================================================================
*/

// Include the header file
#include "DrumPadComponent.h"

// Constructor to initializes drum pads, volume sliders, loop buttons, and audio
DrumPadComponent::DrumPadComponent()
{
    // Register audio formats
    formatManager.registerBasicFormats();
    
    // Initialize audio device with 2 output channels
    deviceManager.initialiseWithDefaultDevices(0, 2);

    // Loop through each drum pad
    for (int i = 0; i < 6; ++i)
    {
        // Listener for the drum pads
        drumPads[i].addListener(this);

        // Default color for the drum pad
        drumPads[i].setColour(juce::TextButton::buttonColourId, defaultPadColor);

        // Show the drum pad on the screen
        addAndMakeVisible(drumPads[i]);

        // Volumn slider range
        volumeSliders[i].setRange(0.0, 1.0, 0.05);

        // Set the starting value of the volume slider to be 80%
        volumeSliders[i].setValue(0.8);

        // Listener for the slider
        volumeSliders[i].addListener(this);

        // Style for the volumne to be increase decrease buttons
        volumeSliders[i].setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);

        // Show the volumn slider on the screen
        addAndMakeVisible(volumeSliders[i]);

        // Text for the button
        loopButtons[i].setButtonText("Loop");

        // Set clicking toggle state
        loopButtons[i].setClickingTogglesState(true);

        // Listener for the loop button
        loopButtons[i].addListener(this);

        // Show the loop buttons on the screen
        addAndMakeVisible(loopButtons[i]);

        // Load the tracks for the drum pads
        loadSample(i, "sample" + juce::String(i + 1) + ".mp3");

        // Add audio call back
        deviceManager.addAudioCallback(&audioSourcePlayer[i]);

        // Listener for audio state changes
        transportSources[i].addChangeListener(this);
    }

    // Start timer
    startTimer(500);
}

DrumPadComponent::~DrumPadComponent()
{
    for (int i = 0; i < 6; ++i)
    {
        // Remove audio source
        transportSources[i].setSource(nullptr);
        
        // Remove change listener
        transportSources[i].removeChangeListener(this);
        
        // Remove audio callbacks
        deviceManager.removeAudioCallback(&audioSourcePlayer[i]);
    }
    // Stop the blinking effect
    stopTimer();
}

void DrumPadComponent::paint(juce::Graphics& g)
{

}

void DrumPadComponent::resized()
{
    for (int i = 0; i < 6; ++i)
    {
        // x-axis
        int x = 10 + (i % 3) * 80;
        
        // y-axis
        int y = 10 + (i / 3) * 80;

        // position of the drum pads
        drumPads[i].setBounds(x, y, 50, 50);

        // position of the volumn sliders to be beside the drum pads
        volumeSliders[i].setBounds(x + 50, y + 5, 30, 40);

        // position of the loop button below the drum pad
        loopButtons[i].setBounds(x, y + 55, 50, 15);
    }
}

void DrumPadComponent::buttonClicked(juce::Button* button)
{
    // Loop through all the drum pads
    for (int i = 0; i < 6; ++i)
    {
        // Check is the button is clicked
        if (button == &drumPads[i])
        {
            // If the drum pad is playing, stop it
            if (transportSources[i].isPlaying())
            {
                // Stop track and reset position to the beginning
                transportSources[i].stop();
                transportSources[i].setPosition(0);

                // Stop the blinking
                stopBlinking(i);

                // Disable loop
                isLooping[i] = false;

                // Update loop state to off
                loopButtons[i].setToggleState(false, juce::dontSendNotification);
            }
            else
            {
                // If not playing, start track
                transportSources[i].setPosition(0);
                transportSources[i].start();

                // Start blinking effect
                isBlinking[i] = true;
            }
        }
        else if (button == &loopButtons[i])
        {
            // update the loop state based on the toggle
            isLooping[i] = loopButtons[i].getToggleState();

            // Console statement for looping of the different drum pads
            DBG("Looping for Pad " + juce::String(i + 1) + (isLooping[i] ? " enabled" : " disabled"));
        }
    }
}

void DrumPadComponent::sliderValueChanged(juce::Slider* slider)
{
    // Loop through all volumn sliders
    for (int i = 0; i < 6; ++i)
    {
        if (slider == &volumeSliders[i])
        {
            // Set the volumn to the corresponding transport source
            transportSources[i].setGain(static_cast<float>(volumeSliders[i].getValue()));

            // Console statement for the change of volumn
            DBG("Pad " + juce::String(i + 1) + " volume set to " + juce::String(volumeSliders[i].getValue()));
        }
    }
}

void DrumPadComponent::timerCallback()
{
    // Toggle blink state for drum pads
    blinkState = !blinkState;

    // Loop thorough all drum pads to update the blinking effect
    for (int i = 0; i < 6; ++i)
    {
        // If the pad is blinking, update its color
        if (isBlinking[i])
        {
            // Based on blinkstate update the color of the drum pad
            if (blinkState)
                drumPads[i].setColour(juce::TextButton::buttonColourId, padColors[i]);
            else
                drumPads[i].setColour(juce::TextButton::buttonColourId, defaultPadColor);

        }
    }
    // Update the UI
    repaint();
}

void DrumPadComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    // Loop through all transport sources to check for changes
    for (int i = 0; i < 6; ++i)
    {
        // If the transport source has finished playing, update accordingly
        if (source == &transportSources[i] && !transportSources[i].isPlaying())
        {
            // If loop is on, restart track
            if (isLooping[i])
            {
                // Restart the track
                transportSources[i].setPosition(0);
                transportSources[i].start();

                // Console statement for loop of drum pad
                DBG("Looping Pad " + juce::String(i + 1));
            }
            else
            {
                // Stop blinking effect, if loop state is off
                stopBlinking(i);

                // Console statement when the drum pad track is finished
                DBG("Pad " + juce::String(i + 1) + " track finished playing");
            }
        }
    }
}

void DrumPadComponent::stopBlinking(int padIndex)
{
    // Stop blinking effect for the drum pad
    isBlinking[padIndex] = false;

    // Set the drum pad to default color
    drumPads[padIndex].setColour(juce::TextButton::buttonColourId, defaultPadColor);
}

void DrumPadComponent::loadSample(int padIndex, const juce::String& fileName)
{
    // current working directory
    juce::File current_directory = juce::File::getCurrentWorkingDirectory();

    // Search for the project root by looking for a unique file or folder
    while (current_directory.exists() && !current_directory.getChildFile("samples").exists())
    {
        current_directory = current_directory.getParentDirectory();
    }

    // If "samples" folder is found, construct the full path
    juce::File file;
    if (current_directory.getChildFile("samples").exists())
    {
        file = current_directory.getChildFile("samples").getChildFile(fileName);
    }


    // Check if the file exists
    if (!file.existsAsFile())
    {
        DBG("File not found: " + file.getFullPathName());
        return;
    }

    // Create an audio reader for the file
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

    // If creation of reader was successful, load the sample
    if (reader != nullptr)
    {
        // Create and audio format reader source
        readerSources[padIndex] = std::make_unique<juce::AudioFormatReaderSource>(reader.release(), true);

        // set it for the transport source
        transportSources[padIndex].setSource(readerSources[padIndex].get(), 0, nullptr, readerSources[padIndex]->getAudioFormatReader()->sampleRate);

        // Set the transport source to the audio source player
        audioSourcePlayer[padIndex].setSource(&transportSources[padIndex]);

        // Console statement if the sample track is loaded
        DBG("Loaded sample: " + file.getFullPathName());
    }
    else
    {
        // Console statement if the sample track is not loaded
        DBG("Failed to load sample: " + file.getFullPathName());
    }
}

