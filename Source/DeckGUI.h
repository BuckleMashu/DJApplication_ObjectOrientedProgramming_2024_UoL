#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    DeckGUI(DJAudioPlayer* player, 
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse, 
           int rC, int gC, int bC);
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

    void buttonClicked (Button *) override;

    void sliderValueChanged (Slider *slider) override;

    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 

    void timerCallback() override;

    int convertToSeconds(const std::string& timeStr);
    std::string getSongsTime(const juce::File chosenFile);

    //variables to be used by other functions
    Slider volSlider;
    Slider repeatSlider;

    WaveformDisplay waveformDisplay;
    float angle = 0.0;
    float handAngle;

    juce::String title = " ";
    std::string songduration = " ";
    std::string checkpointPos = " ";

    float CFValue;
private:
    //to retrieve the colour values for the deck's background
    int rC;
    int gC;
    int bC;

    juce::AudioFormatManager formatManager;

    AudioPlayHead* playHead;
    AudioPlayHead::CurrentPositionInfo currenPositionInfo;

    //to define the deck's listeners
    TextButton playNstopButton{ "PLAY" };
    TextButton repeatButton{ "JUMP TO CHECKPOINT" };

    bool audioStateP;
  
    Slider speedSlider;
    Slider posSlider;

    FileChooser fChooser{"Select a file..."};

    DJAudioPlayer* player; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
