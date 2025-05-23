#include <JuceHeader.h>
#include "PlaylistComponent.h"
using namespace std;

//by taking the players and the decks as inputs, the playlist can interact with them to load songs
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2, DeckGUI* _deckGUI1, DeckGUI* _deckGUI2
) : player1(_player1), player2(_player2), deckGUI1(_deckGUI1), deckGUI2(_deckGUI2)
{
    //define the listener
    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
    
    //define the columns available of the table
    tableComponent.getHeader().addColumn("title", 1, 300);
    tableComponent.getHeader().addColumn("length", 2, 100);
    tableComponent.getHeader().addColumn(" ", 3, 133);
    tableComponent.getHeader().addColumn(" ", 4, 133);
    tableComponent.getHeader().addColumn(" ", 5, 133);
    tableComponent.getHeader().setColour(TableHeaderComponent::backgroundColourId, juce::Colours::white);
    
    tableComponent.setModel(this);
    
    addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(20, 20, 20));

    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);
    g.setFont (14.0f);

}

void PlaylistComponent::resized()
{
    //setting the bounds of the load button and the playlist table
    loadButton.setBounds(0, 0, getWidth(), 20);
    tableComponent.setBounds(0, 20, getWidth(), getHeight()-20);
}

int PlaylistComponent::getNumRows(){
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected){
    if (rowIsSelected) {
        //if the user select a row, it will highlight it with purple, else, it will be dark gray.
        g.fillAll(Colours::mediumpurple);
    }else{
        g.fillAll(juce::Colour(40, 40, 40));
    }
}
void PlaylistComponent::paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected){
    if (columnId == 1)
    {
        //display the audio file's name
        g.setColour(Colours::yellow);
        g.drawText(trackTitles[rowNumber], 2, 0, width, height, Justification::centredLeft, true);
    }
    if (columnId == 2) 
    {
        //display the audio file's duration
        g.setColour(Colours::yellow);
        g.drawText(songDuration[rowNumber], 2, 0, width, height, Justification::centredLeft, true);
    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) {
    //create listeners to load audio into deck1, to load audio into deck2, and to remove the file from the playlist
    if(columnId == 3){
        if(existingComponentToUpdate==nullptr){
            TextButton* btn = new TextButton{"Remove"};
            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            
            btn->addListener(this);
            existingComponentToUpdate=btn;
            btn->onClick = [this] {removeFromPlaylist(); };
        }
    }
    if (columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{ "Deck1" };
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {loadAudioIntoDeck1();};
        }
    }
    if (columnId == 5) {
        if (existingComponentToUpdate == nullptr) {
            String id{ std::to_string(rowNumber) };
            TextButton* btn = new TextButton{ "Deck2"};
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {loadAudioIntoDeck2(); };
        }
    }
    return existingComponentToUpdate;
}


void PlaylistComponent::buttonClicked(Button * button) {
    if (button == &loadButton)
    {
        //to load one audio file into the playlist
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            auto songURL = URL{ chooser.getResult() };
            song.add(songURL);
            //add the necessary detail into arrays, to be displayed in the table
            trackTitles.add(juce::URL::removeEscapeChars(songURL.getFileName()));
            songDuration.add(deckGUI1->getSongsTime(chooser.getResult()));
            tableComponent.updateContent(); //to display changes to the table from the new element of the arrays
        });
    }

    //to save the id of the row the button is clicked. 
    selected = button->getComponentID().getDoubleValue();
}

//these functions rely on int id saved by the variable 'selected' to load or remove the correct audio
void PlaylistComponent::removeFromPlaylist() {
    song.remove(selected);
    trackTitles.remove(selected);
    tableComponent.updateContent();
}

void PlaylistComponent::loadAudioIntoDeck1() {
    player1->loadURL(song[selected]);
    //setting up the required prerequisite when loading a song into the deck 
    deckGUI1->waveformDisplay.loadURL(song[selected]);
    deckGUI1->angle = 0.0;
    deckGUI1->title = trackTitles[selected];
    deckGUI1->songduration = songDuration[selected];
}

void PlaylistComponent::loadAudioIntoDeck2() {
    player2->loadURL(song[selected]);
    //setting up the required prerequisite when loading a song into the deck 
    deckGUI2->waveformDisplay.loadURL(song[selected]);
    deckGUI2->angle = 0.0;
    deckGUI2->title = trackTitles[selected];
    deckGUI2->songduration = songDuration[selected];
}

//by interating the dragged files, this allow multiple files to be added to the playlist at a time
void PlaylistComponent::filesDropped(const StringArray& files, int x, int y) {
    for (int i = 0; i < files.size(); i = i + 1) {
        song.add(URL{ File{files[i]} });
        trackTitles.add(juce::URL::removeEscapeChars(juce::URL{ File{files[i]} }.getFileName()));

        songDuration.add(deckGUI1->getSongsTime(File{ files[i] }));

        tableComponent.updateContent();
    }
}

bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files) {
    return true;
}