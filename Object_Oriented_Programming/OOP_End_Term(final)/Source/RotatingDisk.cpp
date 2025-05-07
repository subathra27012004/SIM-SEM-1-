// The cpp file contains the implementation which handles the rotating disk, to start, stop, reset rotation, adjust speed, and customize the rotation angle.
/*
  ==============================================================================

    RotatingDisk.cpp
    Created: 22 Feb 2025 4:09:23pm
    Author:  Subathra

  ==============================================================================
*/

// Include all the libraries and the header file
#include <JuceHeader.h>
#include "RotatingDisk.h"

// Constructor to set up the rotating disk
RotatingDisk::RotatingDisk() : rotationAngle(0.0f), isRotating(false), rotationSpeed(0.1f), glowPhase(0.0f)
{
    // set Disk size
    setSize(100, 100);

}

void RotatingDisk::paint(juce::Graphics& g)
{
    // Calculate the radius of the rotating circle
    float radius = (getWidth() - 20) / 2;

    // In the red ellipse, set the steps for drawing fine circles.
    float redRadiusStep = 20.0f / 5;


    // Dynamic RGB colors based on a sine wave for smooth cycling
    float r = (std::sin(glowPhase) + 1.0f) * 127.5f;
    float gVal = (std::sin(glowPhase + juce::MathConstants<float>::pi / 3) + 1.0f) * 127.5f;
    float b = (std::sin(glowPhase + juce::MathConstants<float>::pi * 2 / 3) + 1.0f) * 127.5f;

    // X position of the tip
    float tipX = getWidth() / 2 + radius * std::cos(rotationAngle);

    // Y position of the tip
    float tipY = getHeight() / 2 + radius * std::sin(rotationAngle);

    // Combine the RGB into a color for the dynamic glow
    juce::Colour dynamicGlow = juce::Colour((uint8_t)r, (uint8_t)gVal, (uint8_t)b);

    // Create a neon radial gradient
    juce::ColourGradient glowGradient(dynamicGlow, getWidth() / 2, getHeight() / 2,
        juce::Colours::transparentBlack, getWidth(), getHeight(), true);

    // Set the gradient as the fill color 
    g.setGradientFill(glowGradient);

    // Fill the disk area with the gradient
    g.fillEllipse(0, 0, getWidth(), getHeight());

    // Draw a black ellipse on top of the gradient ellipse
    g.setColour(juce::Colours::black);
    g.fillEllipse(10, 10, getWidth() - 20, getHeight() - 20);
    g.drawEllipse(10, 10, getWidth() - 20, getHeight() - 20, 2.0f);

    // Grey color for the fine circles
    g.setColour(juce::Colour(150, 150, 150));

    // Draw fine circles for the black ellipse
    for (float r = (((getWidth() - 20) / 2) / 10); r < (getWidth() - 20) / 2; r += (((getWidth() - 20) / 2) / 10))
    {
        g.drawEllipse(getWidth() / 2 - r, getHeight() / 2 - r, r * 2, r * 2, 0.2f);
    }

    // Draw a small red ellipse in the center
    g.setColour(juce::Colours::red);
    g.fillEllipse(getWidth() / 2 - 20.0f, getHeight() / 2 - 20.0f, 20.0f * 2, 20.0f * 2);

    // Draw fine circles for the red ellipse
    for (float r = redRadiusStep; r < 20.0f; r += redRadiusStep)
    {
        g.drawEllipse(getWidth() / 2 - r, getHeight() / 2 - r, r * 2, r * 2, 0.5f); // Thinner strokes
    }

    // Set the color of the indicator to be grey
    g.setColour(juce::Colour(150, 150, 150));

    // Make a path for the indicator line that rotates.
    juce::Path indicator;

    // Start at the center
    indicator.startNewSubPath(getWidth() / 2, getHeight() / 2);

    // Draw line to the rotating tip
    indicator.lineTo(getWidth() / 2 + radius * std::cos(rotationAngle), getHeight() / 2 + radius * std::sin(rotationAngle));

    // Draw the stroke path
    g.strokePath(indicator, juce::PathStrokeType(2.0f));

    // Color of the tip ellipse
    g.setColour(juce::Colours::white); 

    // Draw the tip ellipse
    g.fillEllipse(tipX - 4.0f, tipY - 4.0f, 4.0f * 2, 4.0f * 2);
}


void RotatingDisk::resized()
{
    // Update the UI
    repaint();
}

void RotatingDisk::startRotation()
{
    // Set isRotating to true to start rotation
    isRotating = true;

    // Start timer
    startTimer(50);

    if (rotationSpeed == 0.0f) {
        // Set a default speed
        rotationSpeed = 0.1f;
    }
}

void RotatingDisk::stopRotation()
{
    // Set isRotating to false to stop rotation
    isRotating = false;

    // Stop timer
    stopTimer();
}

void RotatingDisk::resetRotation()
{
    // Stop rotation
    stopRotation();

    // Reset the angle to be 0
    rotationAngle = 0.0;

    // Update the UI
    repaint();
}

void RotatingDisk::timerCallback()
{
    // If the disk is rotating
    if (isRotating)
    {
        // Increase the rotation angly by rotation speed
        rotationAngle += rotationSpeed;

        // if the angle exceeds 360 degree
        if (rotationAngle >= juce::MathConstants<float>::twoPi) 
            
            // Set the angle back to 0
            rotationAngle -= juce::MathConstants<float>::twoPi;

        // Update the UI
        repaint();
    }

    // Increase the glow phase for color cycle
    glowPhase += 0.1f;

    // If glow phase exceeds twopi
    if (glowPhase > juce::MathConstants<float>::twoPi)

        // Set the glow phase back to 0
        glowPhase -= juce::MathConstants<float>::twoPi;

    // Updates the UI
    repaint();
}

void RotatingDisk::setRotationSpeed(double speed)
{
    // Scale the speed factor
    rotationSpeed = speed * 0.1f;
}

void RotatingDisk::setRotationAngle(float newAngle)
{
    // New rotation angle
    rotationAngle = newAngle;

    // Updates the UI
    repaint();
}
