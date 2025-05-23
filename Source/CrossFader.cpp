#include "CrossFader.h"
#include "../JuceLibraryCode/JuceHeader.h"


Crossfader::Crossfader(DJAudioPlayer* _player1, DJAudioPlayer* _player2, DeckGUI* _deckGUI1, DeckGUI* _deckGUI2
) : player1(_player1),player2(_player2), deckGUI1(_deckGUI1), deckGUI2(_deckGUI2)
{
    //setting up the crossfader slider listener
    addAndMakeVisible(CFSlider);
    CFSlider.addListener(this);

    //modifying the UI of the crossfader slider
    CFSlider.setRange(0.0, 1.0);
    CFSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    CFSlider.setValue(0.5);
    CFSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    CFSlider.setColour(Slider::backgroundColourId, juce::Colours::grey);
    CFSlider.setColour(Slider::trackColourId, juce::Colours::white);

}

Crossfader::~Crossfader()
{
}

void  Crossfader::paint(Graphics& g)
{
    //background of this class 
    g.fillAll(juce::Colour(100 * CFSlider.getValue(), 0, 100 * (1 - CFSlider.getValue())));
    g.setColour(Colours::black);
    g.drawRect(getLocalBounds(), 1);

    //the text above the crossfader slider
    g.setColour(Colours::white);
    g.setFont(14.0f);
    g.drawText("Cross", 5, 5, getWidth(),20, Justification::centredLeft, true);
    g.drawText("Fader", 5, 25, getWidth(), 20, Justification::centredLeft, true);
}

void Crossfader::resized()
{
    CFSlider.setBounds(0, 40, getWidth(), getHeight()-80);
}


void Crossfader::sliderValueChanged(Slider* slider)
{
    if (slider == &CFSlider)
    {
        //retrieve the value of the volume slider from both decks when the crossfader slider is moved 
        float currentV1 = deckGUI1->volSlider.getValue();
        float currentV2 = deckGUI2->volSlider.getValue();

        //Applying the new volume of each players 
        //This retrieving the CFSlider's value,
        //then apply it accordingly in setGain().
        auto mix = slider->getValue();
        player1->setGain(mix * currentV1);
        player2->setGain((1 - mix) * currentV2);

        //Allow the process of each deck changing their player's volume more naturally.
        deckGUI1->CFValue = mix;
        deckGUI2->CFValue = 1 - mix;
    }
}

