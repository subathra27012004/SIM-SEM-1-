// The cpp file contains the implementation of the header file which is the main part of the DJ app which combines the 2 audio players, their controls, a playlist, and a mixer to handle playback and user interaction.

// Include the header file
#include "MainComponent.h"

//==============================================================================
// Contructor to initialise the main component
MainComponent::MainComponent()
    : deckGUI1(&player1, formatManager, thumbCache, juce::Colours::deepskyblue),
    deckGUI2(&player2, formatManager, thumbCache, juce::Colours::red)
{
    // Main window size
    setSize (1100, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        // If accepted, ask for permission and set up audio channels.
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    // Show the decks on the screen
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);

    // Register basic audio file formats
    formatManager.registerBasicFormats();

    // To load a track into Deck 1 when selected from the playlist
    playlistComponent.onLoadToDeck1 = [this](const juce::String& track)
        {
            DBG("Loading track to Deck 1: " + track);

            // Start from the current working directory
            juce::File currentDir = juce::File::getCurrentWorkingDirectory();

            // Search for the "tracks" folder by moving up the directory tree
            while (currentDir.exists() && !currentDir.getChildFile("tracks").exists())
            {
                currentDir = currentDir.getParentDirectory();
            }

            // If "tracks" is found, construct the full path to the track file
            juce::File trackFile;
            if (currentDir.getChildFile("tracks").exists())
            {
                trackFile = currentDir.getChildFile("tracks").getChildFile(track + ".mp3");
            }

            // Check if the file exists
            if (trackFile.exists())
            {
                DBG("Track exists: " + trackFile.getFullPathName());

                // Load the track
                deckGUI1.loadTrack(trackFile.getFullPathName());

                // Show the title in the deck
                deckGUI1.setCurrentTrackTitle(track);
            }
            else
            {
                // Console statement when the track is not found
                DBG("Track file not found: " + trackFile.getFullPathName());
            }
        };


    // To load a track into Deck 2 when selected from the playlist
    playlistComponent.onLoadToDeck2 = [this](const juce::String& track)
        {
            DBG("Loading track to Deck 2: " + track);

            // Start from the current working directory
            juce::File currentDir = juce::File::getCurrentWorkingDirectory();

            // Search for the "tracks" folder by moving up the directory tree
            while (currentDir.exists() && !currentDir.getChildFile("tracks").exists())
            {
                currentDir = currentDir.getParentDirectory();
            }

            // If "tracks" is found, construct the full path to the track file
            juce::File trackFile;
            if (currentDir.getChildFile("tracks").exists())
            {
                trackFile = currentDir.getChildFile("tracks").getChildFile(track + ".mp3");
            }

            // Check if the file exists
            if (trackFile.exists())
            {
                DBG("Track exists: " + trackFile.getFullPathName());

                // Load the track
                deckGUI2.loadTrack(trackFile.getFullPathName());

                // Show the title in the deck
                deckGUI2.setCurrentTrackTitle(track);
            }
            else
            {
                // Console statement when the track is not found
                DBG("Track file not found: " + trackFile.getFullPathName());
            }
        };

    // Show the playlist component on the screen
    addAndMakeVisible(playlistComponent);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // Add player 1 and player 2 into the mixer
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

    // Prepare the mixer to play at the given sample rate
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Get audio from the mixer
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // Remove all audio sources from the mixer
    mixerSource.removeAllInputs();

    // Release the resources
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();

}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // Background fill
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void MainComponent::resized()
{
    // Position and size of deck 1
    deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() / 2);
   
    // Position and size of deck 2
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() / 2);
   
    // Position and sizr of playlist component
    playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
}