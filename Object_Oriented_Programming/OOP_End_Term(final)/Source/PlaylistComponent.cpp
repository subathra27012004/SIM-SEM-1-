// The cpp file contains the implementations which manages a playlist UI with functionalities such as displaying tracks, filtering, importing music, and interacting with buttons for loading tracks to decks.
/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 30 Jan 2025 7:33:53pm
    Author:  Subathra

  ==============================================================================
*/

// Include all the libraries and the header file
#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <algorithm>

//==============================================================================
// Constructor
PlaylistComponent::PlaylistComponent()
{
    // Add title column in the table
    tableComponent.getHeader().addColumn("Track title", 1, 548);

    // Add duration column in the table
    tableComponent.getHeader().addColumn("Duration(hhmmss)", 2, 170);

    // Add deck 1 load column in the table
    tableComponent.getHeader().addColumn("Load to", 3, 150);

    // Add deck 2 load column in the table
    tableComponent.getHeader().addColumn("Load to", 4, 150);

    // Add delete column in the table
    tableComponent.getHeader().addColumn("Delete", 5, 60);

    // Set the row height 
    tableComponent.setRowHeight(40);

    // Set the model for the table
    tableComponent.setModel(this);

    // Show the table on the screen
    addAndMakeVisible(tableComponent);

    // Set up Import Music Button
    importButton.setButtonText("Import Music");

    // Listener for the import button
    importButton.addListener(this);

    // Show the import button on the screen
    addAndMakeVisible(importButton);

    // Show the search box on the screen
    addAndMakeVisible(searchBox);

    // Listener for the search box
    searchBox.addListener(this);

    // Placeholder text
    searchBox.setTextToShowWhenEmpty(juce::String{ "Search" }, juce::Colours::black);

    // Set text color to black
    searchBox.setColour(juce::TextEditor::textColourId, juce::Colours::black);

    // Background fill for the search box
    searchBox.setColour(juce::TextEditor::backgroundColourId, juce::Colours::lightblue);

    // Font style for the search box
    searchBox.setFont(juce::Font(15.0f, juce::Font::bold));

    // text justification in search box
    searchBox.setJustification(juce::Justification::centredLeft);
}

// Destructor
PlaylistComponent::~PlaylistComponent()
{
    // Remove model from the table when the table is destroyed
    tableComponent.setModel(nullptr);
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    // Background fill
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    // Set color to grey for the outer boarder line
    g.setColour (juce::Colours::grey);

    // draw an outline around the component
    g.drawRect (getLocalBounds(), 1);
}

int PlaylistComponent::getNumRows()
{
    // Returns the number of rows in the table
    if (userFilteredTrackTitles.empty())
    {
        return trackTitles.size();
    }
    else
    {
        return userFilteredTrackTitles.size();
    }
}



void PlaylistComponent::paintRowBackground(juce::Graphics& g, int	rowNumber, int	width, int	height, bool rowIsSelected)
{
    // If row is selected
    if (rowIsSelected)
    {
        // Fill the row with lightblue
        g.fillAll(juce::Colours::lightblue);
    }
    else {
        // Else fill the row with white
        g.fillAll(juce::Colours::white);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    // To store the list of track titles that should be used
    const std::vector<std::string>* titlesToUse;

    // Decide whether to use the full track list or the filtered list
    if (userFilteredTrackTitles.empty())
    {
        // Use the full track list if no filtering is applied
        titlesToUse = &trackTitles;
    }
    else
    {
        // Use filtered list if search is applied
        titlesToUse = &userFilteredTrackTitles;
    }

    // Track duration column
    if (columnId == 2)
    {
        // Draw track duration in the corresponding cell
        g.drawText(trackDurations[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    // Track title column
    else
    {
        // Draw the track title in the corresponding cell
        g.drawText((*titlesToUse)[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
}

// Create and update components for the buttons in the table
juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    if (columnId == 3 || columnId == 4 || columnId == 5)
    {
        if (existingComponentToUpdate == nullptr)
        {
            // If it is the delete column
            if (columnId == 5)
            {
                // Drawable Button to put the image in the button
                auto* imgButton = new juce::DrawableButton("DeleteButton", juce::DrawableButton::ImageOnButtonBackground);

                // Start from the current working directory
                juce::File currentDir = juce::File::getCurrentWorkingDirectory();

                // Search for the "samples" folder by moving up the directory tree
                while (currentDir.exists() && !currentDir.getChildFile("samples").exists())
                {
                    currentDir = currentDir.getParentDirectory();
                }

                // If "samples" is found, construct the full path to "delete_icon.png"
                juce::File deleteIconFile;
                if (currentDir.getChildFile("samples").exists())
                {
                    deleteIconFile = currentDir.getChildFile("samples").getChildFile("delete_icon.png");
                }
                
                // Log the directory for debugging
                juce::Logger::writeToLog("Looking for delete icon at: " + deleteIconFile.getFullPathName());

                // Ensure the file exists
                if (!deleteIconFile.existsAsFile())
                {

                    // Console statement when the icon is not found
                    juce::Logger::writeToLog("Error: Delete icon not found at " + deleteIconFile.getFullPathName());
                    
                    // Return null is the file does not exists
                    return nullptr;
                }

                // Load the image
                juce::Image deleteImage = juce::ImageFileFormat::loadFrom(deleteIconFile);

                // If the image is valid
                if (deleteImage.isValid())
                {
                    auto drawableImage = std::make_unique<juce::DrawableImage>();
                    drawableImage->setImage(deleteImage);

                    // Set the image in the button
                    imgButton->setImages(drawableImage.get());
                }

                // Set component id to row column format
                imgButton->setComponentID(juce::String(rowNumber) + "-" + juce::String(columnId));
                
                // Listerner for the button
                imgButton->addListener(this);
                
                // Update the existing component
                existingComponentToUpdate = imgButton;
            }
            else
            {
                // Pointer to a TextButton
                juce::TextButton* btn;

                if (columnId == 3)
                {
                    // Button for deck 1
                    btn = new juce::TextButton{ "Deck 1" };
                }
                else
                {
                    // Button for deck 2
                    btn = new juce::TextButton{ "Deck 2" };
                }

                // unique ID to the button using row and column info
                btn->setComponentID(juce::String(rowNumber) + "-" + juce::String(columnId));

                //  Listener for the button
                btn->addListener(this);

                // Update the existing component
                existingComponentToUpdate = btn;
            }

        }
        // Update the existing component
        return existingComponentToUpdate;
    }
    // Return null if no component is needed
    return nullptr;
}

void PlaylistComponent::resized()
{
    // Size of the search box
    searchBox.setBounds(10, 10, getWidth() - 2 * 10, 30);

    // Size of the tablecomponent
    tableComponent.setBounds(10, 45, getWidth() - 2 * 10, getHeight() - 30 - 65);

    // Size of the import button
    importButton.setBounds(10, getHeight() - 45, getWidth() - 10 * 2, 40);
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    // If import button is clicked
    if (button == &importButton)
    {
        // Call the function
        importMusic();
        return;
    }

    // Get the ID of the button
    juce::String buttonID = button->getComponentID();
    juce::StringArray tokens;

    // Split the component ID into row and column
    tokens.addTokens(buttonID, "-", "");

    // Ensure there are 2 parts
    if (tokens.size() != 2) return;

    // Extract the row number
    int row = tokens[0].getIntValue();

    // Extract the column ID
    int columnId = tokens[1].getIntValue();
    
    // A non-const reference to store the track list
    const std::vector<std::string>* titlesToUsePtr = &userFilteredTrackTitles;

    // If the filtered list is empty, use the full track list instead
    if (titlesToUsePtr->empty())
    {
        titlesToUsePtr = &trackTitles;
    }

    // Use *titlesToUsePtr to access the selected track list
    const auto& titlesToUse = *titlesToUsePtr;

    // Ensure valid row
    if (row < 0 || row >= titlesToUse.size()) return;

    // Find the actual index in trackTitles
    auto it = std::find(trackTitles.begin(), trackTitles.end(), titlesToUse[row]);
    
    // Ensure the track exists
    if (it == trackTitles.end()) return;

    // Get original index
    int actualIndex = std::distance(trackTitles.begin(), it);

    // If deck 1 button is clicked
    if (columnId == 3)
    {
        // Call deck 1 function
        if (onLoadToDeck1) onLoadToDeck1(trackTitles[actualIndex]);
    }

    // If deck 2 button is clicked
    else if (columnId == 4)
    {
        // Call deck 2 function
        if (onLoadToDeck2) onLoadToDeck2(trackTitles[actualIndex]);
    }

    // If delete button is clicked
    else if (columnId == 5)
    {
        // Remove track from original list
        trackTitles.erase(trackTitles.begin() + actualIndex);

        // Remove duration from the original list
        trackDurations.erase(trackDurations.begin() + actualIndex);

        // If filtering is active, also remove from filtered list
        if (!userFilteredTrackTitles.empty())
        {
            userFilteredTrackTitles.erase(userFilteredTrackTitles.begin() + row);
        }

        // Refresh UI
        tableComponent.updateContent();
    }
}

void PlaylistComponent::importMusic()
{
    // Allow file selection
    auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::canSelectMultipleItems;

    // Launch file chooser
    fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
        {
            // Get the selected files
            auto chosenFiles = chooser.getResults();

            // Loop through each file
            for (const auto& file : chosenFiles)
            {
                // Get track name without the extension
                std::string trackName = file.getFileNameWithoutExtension().toStdString();

                // Check if the track is already in the playlist
                if (std::find(trackTitles.begin(), trackTitles.end(), trackName) != trackTitles.end())
                {
                    // Alert message is there are duplicate tracks in the playlist
                    juce::AlertWindow::showMessageBoxAsync(
                        juce::AlertWindow::WarningIcon,
                        "                           Duplicate Track",
                        "        The track '" + trackName + "' is already in the playlist.",
                        "OK"
                    );
                    continue;
                }

                // Track folder path
                juce::File tracksFolder = juce::File::getSpecialLocation(juce::File::currentExecutableFile)
                                         .getParentDirectory()
                                         .getParentDirectory()
                                         .getParentDirectory()
                                         .getParentDirectory()
                                         .getParentDirectory()
                                         .getParentDirectory()
                                         .getChildFile("tracks");


                // Ensure the tracks folder exists
                if (!tracksFolder.exists())
                    tracksFolder.createDirectory();

                // Create the destination file path inside the tracks folder
                juce::File destinationFile = tracksFolder.getChildFile(file.getFileName());

                //  Check is the file exists
                if (!destinationFile.existsAsFile())
                {
                    // Copy the file to the tracks folder
                    bool success = file.copyFileTo(destinationFile);

                    if (!success)
                    {
                        // Console statement if the copying does not work
                        juce::Logger::writeToLog("Failed to copy track to tracks folder: " + destinationFile.getFullPathName());
                        continue;
                    }
                }

                // Console statement for the track being copied successfully
                juce::Logger::writeToLog("Track copied to: " + destinationFile.getFullPathName());

                // Set up formatmanager to prepare for audio track
                juce::AudioFormatManager formatManager;

                // Register audio formats
                formatManager.registerBasicFormats();

                // Create audio reader for the copied file
                juce::AudioFormatReader* reader = formatManager.createReaderFor(destinationFile);
                if (reader != nullptr)
                {
                    // Check duration of the tracks in seconds
                    double InSec = static_cast<double>(reader->lengthInSamples) / reader->sampleRate;
                    
                    // Convert seconds into min and sec format
                    int min = static_cast<int>(InSec) / 60;
                    int sec = static_cast<int>(InSec) % 60;
                    
                    // Duration as a string
                    std::string formattedDuration = juce::String::formatted("%02d:%02d", min, sec).toStdString();

                    // Store track details
                    trackTitles.push_back(trackName);
                    trackDurations.push_back(formattedDuration);
                    trackFilePaths.push_back(destinationFile.getFullPathName().toStdString()); // Store new path

                    // Free up memory
                    delete reader;
                }
            }
            // Update content
            tableComponent.updateContent();
        });
}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor&)
{
    // Search text entered by the user
    std::string searchTerm = searchBox.getText().toStdString();
    
    // Clear the previous search
    userFilteredTrackTitles.clear();

    // If search box is empty
    if (searchTerm.empty())
    {
        // Update content
        tableComponent.updateContent();
        return;
    }

    // Perform case-insensitive search
    for (const auto& song : trackTitles)
    {
        //  Convert search term and song names to lowercase
        std::string lowerSong = song;
        std::string lowerSearchTerm = searchTerm;

        std::transform(lowerSong.begin(), lowerSong.end(), lowerSong.begin(), ::tolower);
        std::transform(lowerSearchTerm.begin(), lowerSearchTerm.end(), lowerSearchTerm.begin(), ::tolower);

        // Check if the song name contains the search term
        if (lowerSong.find(lowerSearchTerm) != std::string::npos)
        {
            // If there is a match, add it to the filtered list 
            userFilteredTrackTitles.push_back(song);
        }
    }

    // Update content
    tableComponent.updateContent();
}
