// The header file contains the functions which manages a playlist UI with functionalities such as displaying tracks, filtering, importing music, and interacting with buttons for loading tracks to decks.
/*
  ==============================================================================

    PlaylistComponent.h
    Created: 30 Jan 2025 7:33:53pm
    Author:  Subathra

  ==============================================================================
*/

// Include all the libraries
#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

//==============================================================================
/*
*/
// Include all JUCE classes 
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener
{
public:
    // Constructor
    PlaylistComponent();

    // Destructor
    ~PlaylistComponent() override;

    // Paint function to draw on the screen
    void paint (juce::Graphics&) override;

    // To resize the components on the screen
    void resized() override;

    // The number of rows in the table
    int getNumRows() override;

    // Paint the background of each row
    void paintRowBackground(juce::Graphics& g,
                            int	rowNumber,
                            int	width,
                            int	height,
                            bool rowIsSelected) override;

    // Paint each cell in the table
    void paintCell(juce::Graphics& g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool	rowIsSelected) override;

    // Refresh a custome component inside a table cell
    juce::Component* refreshComponentForCell(int	rowNumber,
                                       int	columnId,
                                       bool	isRowSelected,
                                       Component* existingComponentToUpdate) override;

    // Called when a button is clicked
    void buttonClicked(juce::Button* button) override;

    // Function for the search box
    void textEditorTextChanged(juce::TextEditor&) override;

    // Callback function for loading track to Deck 1
    std::function<void(const juce::String&)> onLoadToDeck1;

    // Callback function for loading track to Deck 2
    std::function<void(const juce::String&)> onLoadToDeck2;

private:

    // Import music into the playlist table
    void importMusic();

    // Text box for the searching of tracks
    juce::TextEditor searchBox;

    // Store the title of tracks after user filter
    std::vector<std::string> userFilteredTrackTitles;

    // Store the duration of the track
    std::vector<std::string> trackDurations;

    // Store file paths for all tracks
    std::vector<std::string> trackFilePaths;

    // Table which shows the playlist of tracks
    juce::TableListBox tableComponent;

    // Store the titles of all tracks
    std::vector<std::string> trackTitles;

    // Import button
    juce::TextButton importButton{ "IMPORT MUSIC" };

    // File choose dialog to select files
    juce::FileChooser fChooser{ "Select a file..." };

    // Prevents copying this class to avoid memory issues
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
