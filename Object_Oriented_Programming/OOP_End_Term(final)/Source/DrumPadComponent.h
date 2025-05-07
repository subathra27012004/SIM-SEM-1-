// The header file contains the functions of the management of the drum pads with audio playback, volume control, and looping functionality.

/*
  ==============================================================================

    DrumPadComponent.h
    Created: 24 Feb 2025 11:07:32pm
    Author:  Subathra

  ==============================================================================
*/
// Include the juce library
#pragma once

#include <JuceHeader.h>

// Include all the JUCE classes 
class DrumPadComponent : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::ChangeListener
{
public:
    // Constructor
    DrumPadComponent();

    // Destructor
    ~DrumPadComponent() override;

    // Paint function to draw on the screen
    void paint(juce::Graphics&) override;

    // To resize the components on the screen
    void resized() override;

    // Called when a button is clicked
    void buttonClicked(juce::Button* button) override;

    // Called when slider value changes
    void sliderValueChanged(juce::Slider* slider) override;

    // Update UI at regular intervals
    void timerCallback() override;

    // Handles audio changes
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:

    // Load audio file
    void loadSample(int padIndex, const juce::String& fileName);

    // Stop the blinking effect
    void stopBlinking(int padIndex);

    // Keeps track of the blinking state
    bool isBlinking[6] = { false };

    // Controls blinking effect
    bool blinkState = false;

    // Keeps track of loop state
    bool isLooping[6] = { false };

    // Butons for drun pads
    juce::TextButton drumPads[6];

    // Volumn sliders for each drum pad
    juce::Slider volumeSliders[6];

    // Toggle buttons for looping
    juce::ToggleButton loopButtons[6];

    // Manages audio file formats
    juce::AudioFormatManager formatManager;

    // Handles audio track
    juce::AudioTransportSource transportSources[6];

    // Store audio sources
    std::unique_ptr<juce::AudioFormatReaderSource> readerSources[6];

    // Manages audio devices
    juce::AudioDeviceManager deviceManager;

    // Handles audio track for eaach drum pad
    juce::AudioSourcePlayer audioSourcePlayer[6];

    // Default color of the drum pad
    juce::Colour defaultPadColor = juce::Colours::black;

    // Color for each drum pad
    juce::Colour padColors[6] = { juce::Colours::red, juce::Colours::blue, juce::Colours::green,
                                  juce::Colours::orange, juce::Colours::purple, juce::Colours::yellow };

    // Prevents copying this class to avoid memory issues
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumPadComponent)
};
