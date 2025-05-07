// The cpp file contains the implementation of the DJ deck, handling playback, UI and file loading. 
/*
  ==============================================================================

    DeckGUI.cpp
    Created: 28 Jan 2025 10:53:35pm
    Author:  Subathra

  ==============================================================================
*/

// Include the header file and juce library
#include <JuceHeader.h>
#include "DeckGUI.h"

// Constructor for the class
DeckGUI::DeckGUI(DJAudioPlayer* _player,
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse,
    juce::Colour color
) : player(_player),
waveformDisplay(formatManagerToUse, cacheToUse, color)
{
    // Show the play button on the screen
    addAndMakeVisible(playButton);

    // Show the stop button on the screen
    addAndMakeVisible(stopButton);

    // Show the loop button on the screen
    addAndMakeVisible(loopButton);

    // Show the load button on the screen
    addAndMakeVisible(loadButton);

    // Show volumn slider on the screen
    addAndMakeVisible(volSlider);

    // Show speed slider on the screen
    addAndMakeVisible(speedSlider);

    // Show position slider on the screen
    addAndMakeVisible(posSlider);

    // Show volumn slider label on the screen
    addAndMakeVisible(volSliderLabel);

    // Show speed slider label on the screen
    addAndMakeVisible(speedSliderLabel);

    // Show position slider label on the screen
    addAndMakeVisible(posSliderLabel);

    // Show the waveform on the screen
    addAndMakeVisible(waveformDisplay);

    // Show the rotating disk on the screen
    addAndMakeVisible(rotatingDisk);

    // Show the drum pad component on the scrren
    addAndMakeVisible(DrumPad);

    // Listener for the play button
    playButton.addListener(this);

    // Listener for the stop button
    stopButton.addListener(this);

    // Listerner for the loop button
    loopButton.addListener(this);

    // Listener for the load Button
    loadButton.addListener(this);

    // Listener for the volumn slider
    volSlider.addListener(this);

    // Listener for the speed slider
    speedSlider.addListener(this);

    // Listener for the position slider
    posSlider.addListener(this);

    // Range of the volumn slider
    volSlider.setRange(0.0, 1.0);

    // Range of the speed slider
    speedSlider.setRange(0.1, 100.0);

    // Range of the position slider
    posSlider.setRange(0.0, 1.0);

    // Volumn slider label
    volSliderLabel.setText("Volume", juce::NotificationType::dontSendNotification);
    volSliderLabel.setFont(juce::Font(14.0f));

    // Speed slider label
    speedSliderLabel.setText("Speed", juce::NotificationType::dontSendNotification);
    speedSliderLabel.setFont(juce::Font(14.0f));

    // Position slider label
    posSliderLabel.setText("Position", juce::NotificationType::dontSendNotification);
    posSliderLabel.setFont(juce::Font(14.0f));

    // Make the values of volumn slider to be 2 decimal places
    volSlider.setNumDecimalPlacesToDisplay(2);

    // Default volumn to be 80%
    volSlider.setValue(0.8);

    // Make the volumn slider to be vertical bar with no text box
    volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    // Make the values of speed slider to be 2 decimal places
    speedSlider.setNumDecimalPlacesToDisplay(2);
    
    // Make the speed slider to be vertical bar with no text box
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    // Make the values of position slider to be 2 decimal places
    posSlider.setNumDecimalPlacesToDisplay(2);
    
    // Make the position slider to be vertical bar with no text box
    posSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    // Slider color to be light grey
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::lightslategrey);

    // Play button color to be green
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);

    // Stop button color to be red
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);

    // Load button color to be yellow
    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colours::goldenrod);

    // Start timer
    startTimer(200);
}

DeckGUI::~DeckGUI()
{
    // Stop timer when the app closes
    stopTimer();
}



void DeckGUI::paint(juce::Graphics& g)
{
    // Background fill
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Font color to be white
    g.setColour(juce::Colours::white);

    // Size and bold the font
    g.setFont(juce::Font(15.0f, juce::Font::bold));

    // Show the track title in the deck
    g.drawText(currentTrackTitle, -10, getHeight() - 230, getWidth() - 20, 20, juce::Justification::centred);

}

void DeckGUI::resized()
{
    // Size of row
    double rowH = getHeight() / 9;

    // Postion of the waveform
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);

    // Position of the volumn slider
    volSlider.setBounds(getWidth() / 3 - 150, rowH * 2 + 15, 30, getHeight() / 4 + 85);

    // Postion of the speed slider
    speedSlider.setBounds(getWidth() / 3 - 110, rowH * 2 + 15, 30, getHeight() / 4 + 85);

    // Postion of the position slider
    posSlider.setBounds(getWidth() / 3 - 70, rowH * 2 + 15, 30, getHeight() / 4 + 85);

    // Postion of the rotating disk
    rotatingDisk.setBounds(getWidth() / 3 - 30, rowH * 2 + 20, 150, 150);

    // Postion of the drum pads
    DrumPad.setBounds(getWidth() / 3 + 115, rowH * 2 + 15, 250, 200);

    // Postion of the play button
    playButton.setBounds(((getWidth() - (getWidth() * 0.6)) / 5) + 40, rowH * 8 - 5, (getWidth() * 0.6) / 4, rowH);

    // Postion of the stop button
    stopButton.setBounds(((getWidth() - (getWidth() * 0.6)) / 5) + 50 + (getWidth() * 0.6) / 4, rowH * 8 - 5, (getWidth() * 0.6) / 4, rowH);

    // Postion of the load button
    loadButton.setBounds(((getWidth() - (getWidth() * 0.6)) / 5) + 60 + ((getWidth() * 0.6) / 4) * 2, rowH * 8 - 5, (getWidth() * 0.6) / 4, rowH);

    // Postion of the loop button
    loopButton.setBounds(((getWidth() - (getWidth() * 0.6)) / 5) + 70 + ((getWidth() * 0.6) / 4) * 3, rowH * 8 - 5, (getWidth() * 0.6) / 4, rowH);

    // Postion of the volumn slider label
    volSliderLabel.setBounds(volSlider.getX() - 70, volSlider.getY() + volSlider.getHeight() / 2 - 142, 100, 20);

    // Rotate the volumn slider label to be vertical
    volSliderLabel.setTransform(
        juce::AffineTransform::rotation(-juce::MathConstants<float>::halfPi)
        .translated(volSlider.getX() - 15, volSlider.getY() + volSlider.getHeight() / 2 + 20)
    );

    // Make the label center
    volSliderLabel.setJustificationType(juce::Justification::centred);

    // Position of the speed slider label
    speedSliderLabel.setBounds(speedSlider.getX() - 110, speedSlider.getY() + speedSlider.getHeight() / 2 - 142, 100, 20);

    // Rotate the speed slider label to be vertical
    speedSliderLabel.setTransform(
        juce::AffineTransform::rotation(-juce::MathConstants<float>::halfPi)
        .translated(speedSlider.getX() - 15, speedSlider.getY() + speedSlider.getHeight() / 2 + 20)
    );

    // Make the label center
    speedSliderLabel.setJustificationType(juce::Justification::centred);

    // Postion of the position slider label
    posSliderLabel.setBounds(posSlider.getX() - 148, posSlider.getY() + posSlider.getHeight() / 2 - 142, 100, 20);

    // Rotate the position slider label to be vertical
    posSliderLabel.setTransform(
        juce::AffineTransform::rotation(-juce::MathConstants<float>::halfPi)
        .translated(posSlider.getX() - 15, posSlider.getY() + posSlider.getHeight() / 2 + 20)
    );

    // Make the label center
    posSliderLabel.setJustificationType(juce::Justification::centred);
}


void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        // Console statement when play button is clicked
        DBG("Play Button was clicked");

        // Start playback
        player->start();

        // Start the rotation of the disk when the play button is clicked
        rotatingDisk.startRotation();
    }
    if (button == &stopButton)
    {
        // Console statement when stop button is clicked
        DBG("Stop Button was clicked");

        // Stop playback
        player->stop();

        // Stop the rotation of disk when the stop button is clicked
        rotatingDisk.stopRotation();
    }
    if (button == &loopButton)
    {
        // loop state
        looping = !looping;

        // Set looping state in the player
        player->loop(looping);

        if (looping)
        {
            // Console statement for the loop button
            DBG("Loop is on");

            // Set the button color to be blue when the loop is on
            loopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);
        }
        else
        {
            // Console statement for the lopp button
            DBG("Loop is off");

            // Set the button color back to normal when the loop is off
            loopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
        }

        // Update the UI
        repaint();
    }
    if (button == &loadButton)
    {
        // Allow to select files
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;

        // File chooser to select files
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                // Get the file
                auto chosenFile = chooser.getResult();

                // Load the file into the player
                player->loadURL(juce::URL{ chosenFile });

                // Load the file into the waveform display
                waveformDisplay.loadURL(juce::URL{ chosenFile });

                // Reset the rotation disk to the starting position
                rotatingDisk.resetRotation();

                // Get the track title
                juce::String trackTitle = chosenFile.getFileNameWithoutExtension();

                // Display the track title
                setCurrentTrackTitle(trackTitle);

                // Update the UI
                repaint();
            });
    }
}


void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        // Adjust the volumn 
        player->setGain(slider->getValue());
    }
    else if (slider == &speedSlider)
    {
        // Adjust the speed
        player->setSpeed(slider->getValue());

        // Adjust the speed of the disk
        rotatingDisk.setRotationSpeed(slider->getValue());
    }
    else if (slider == &posSlider)
    {
        // Adjust the postion
        player->setPostionRelative(slider->getValue());

        // Adjust the postion of the disk
        rotatingDisk.setRotationAngle(slider->getValue() * juce::MathConstants<float>::twoPi);
    }
}

// To check if a file is dropped
bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    // Console statment when a file is dragged
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    // Only 1 file is dropped
    if (files.size() == 1)
    {
        // Get the dropped file
        juce::File file{ files[0] };

        // If the file is available
        if (file.existsAsFile())
        {
            // Load the track into the deck
            player->loadURL(juce::URL{ file });
            // Update waveform
            waveformDisplay.loadURL(juce::URL{ file });
        }
    }
}

void DeckGUI::timerCallback()
{
    // Get current playback position
    double pos = player->getPositionRelative();

    // Update waveform display position
    if (pos >= 0.0 && pos <= 1.0)
    {
        waveformDisplay.setPositionRelative(pos);
    }

    // Check if the track has ended and stop rotation if loop mode is off
    if (pos >= 1.0)
    {
        // When loop mode is on
        if (looping)
        {
            // Restart from the beginning
            player->setPostionRelative(0.0);
            
            // Start playback again
            player->start();
        }
        else
        {
            // Stop playback when loop mode is off
            player->stop();
            
            // Stop disk rotation
            rotatingDisk.stopRotation();
        }
    }
}



void DeckGUI::loadTrack(const juce::String& filePath)
{
    // Load the track into the deck
    player->loadURL(juce::URL{ juce::File{filePath} });
    
    // Display the waveform
    waveformDisplay.loadURL(juce::URL{ juce::File{filePath} });
    
    // Reset the disk to the starting position
    rotatingDisk.resetRotation();
    
    // Update the UI
    repaint();
}

void DeckGUI::setCurrentTrackTitle(const juce::String& title)
{

    // Set the current track title to the title of the file
    currentTrackTitle = title;
    
    // Update the UI
    repaint();
}