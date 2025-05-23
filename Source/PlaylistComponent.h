#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

#include "DJAudioPlayer.h"
#include "DeckGUI.h"

class PlaylistComponent  : public juce::Component, public TableListBoxModel, public Button::Listener, public juce::TextEditor::Listener,
                           public FileDragAndDropTarget
{
public:
    PlaylistComponent(DJAudioPlayer* player1, DJAudioPlayer* player2, DeckGUI* deckGUI1, DeckGUI* deckGUI2);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    int getNumRows() override;
    void paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    
    void buttonClicked(Button * button) override;
    //defining the function to remove songs from the playlist
    void removeFromPlaylist();

    //defining the functions to add an audio to their respective players.
    void loadAudioIntoDeck1();
    void loadAudioIntoDeck2();

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    
private:
    juce::AudioFormatManager formatManager;

    int selected;

    DJAudioPlayer* player1;
    DJAudioPlayer* player2;

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    TableListBox tableComponent;

    //define arrays to store necessary informations of audios in the playlist
    juce::Array<juce::String> trackTitles;
    juce::Array<juce::URL> song;
    juce::Array<std::string> songDuration;

    //define the listener
    TextButton loadButton{ "LOAD A SONG INTO THE PLAYLIST" };
    
    FileChooser fChooser{ "Select a file...", juce::File(), "*.mp3;*.wav;*.aiff"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
