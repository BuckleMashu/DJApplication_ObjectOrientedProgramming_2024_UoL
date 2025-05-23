#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"

class Crossfader : public Component,
    public Slider::Listener
{
public:
    Crossfader(DJAudioPlayer* player1, DJAudioPlayer* player2, DeckGUI* deckGUI1, DeckGUI* deckGUI2);
    ~Crossfader();

    void paint(Graphics&) override;
    void resized() override;

    //define the interactables' listeners
    void sliderValueChanged(Slider* slider) override;

private:
    //define the interactables
    Slider CFSlider;

    //define variables for the class's inputs 
    DJAudioPlayer* player1;
    DeckGUI* deckGUI1;
    DJAudioPlayer* player2;
    DeckGUI* deckGUI2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Crossfader)
};