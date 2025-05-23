#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "CrossFader.h"

class MainComponent   : public AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (Graphics& g) override;
    void resized() override;

private:
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100}; 

    DJAudioPlayer player1{formatManager};
    //added the colour format at the end to define the deck's background colour
    DeckGUI deckGUI1{&player1, formatManager, thumbCache, 100,0,0}; 

    DJAudioPlayer player2{formatManager};
    //added the colour format at the end to define the deck's background colour
    DeckGUI deckGUI2{&player2, formatManager, thumbCache,0,0,100}; 

    //define the crossfader class
    Crossfader crossFader{&player1,&player2,&deckGUI1,&deckGUI2};

    MixerAudioSource mixerSource; 
    

    PlaylistComponent playlistComponent{ &player1,&player2, &deckGUI1, &deckGUI2 };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
