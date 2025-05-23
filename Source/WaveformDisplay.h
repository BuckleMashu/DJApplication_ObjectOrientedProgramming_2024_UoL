#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class WaveformDisplay    : public Component, 
                           public ChangeListener
{
public:
    WaveformDisplay( AudioFormatManager & 	formatManagerToUse,
                    AudioThumbnailCache & 	cacheToUse );
    ~WaveformDisplay();

    void paint (Graphics&) override;
    void resized() override;

    void changeListenerCallback (ChangeBroadcaster *source) override;

    void loadURL(URL audioURL);

    void setPositionRelative(double pos);
    //defined in public for the checkpoint system's slider to move the red vertical line
    double checkPointPosition = 0;
private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    //to store the coordinate, to move the green box accordingly
    double position;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
