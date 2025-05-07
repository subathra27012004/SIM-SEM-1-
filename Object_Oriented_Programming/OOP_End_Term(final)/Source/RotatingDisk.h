// The header file contains the functions which handles the rotating disk, to start, stop, reset rotation, adjust speed, and customize the rotation angle.
/*
  ==============================================================================

    RotatingDisk.h
    Created: 22 Feb 2025 4:09:23pm
    Author:  Subathra

  ==============================================================================
*/
// Include the juce library
#pragma once

#include <JuceHeader.h>

// Include all JUCE classes 
class RotatingDisk : public juce::Component, public juce::Timer
{
public:
    // Constructor
    RotatingDisk();

    // Paint function to draw on the screen
    void paint(juce::Graphics& g) override;

    // To resize the components on the screen
    void resized() override;

    // Start the rotation of the disk
    void startRotation();

    // Stop the rotation of the disk
    void stopRotation();

    // Reset the rotatation of the disk
    void resetRotation();

    // Timer callback to update the rotation
    void timerCallback() override;

    // Adjust speed dynamically
    void setRotationSpeed(double speed);

    // Adjust the rotation angle
    void setRotationAngle(float newAngle);


private:
    // Variable to check if the disk is rotating
    bool isRotating;

    // Current rotation angle
    double rotationAngle = 0.0;

    // Current rotation speed
    double rotationSpeed = 0.0;

    // Variable to store the glow effect for the disk
    float glowPhase;

    // Prevents copying this class to avoid memory issues
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotatingDisk)
};