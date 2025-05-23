#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(AudioFormatManager & 	formatManagerToUse,
                                 AudioThumbnailCache & 	cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0)
                          
{
  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    //change the waveform's background
    g.fillAll (juce::Colour(40, 40, 40));

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    //to set the waveform's colour as yellow
    g.setColour (Colours::yellow);
    if(fileLoaded)
    {
      //to display the waveform if an audio file is loaded
      audioThumb.drawChannel(g, 
        getLocalBounds(), 
        0, 
        audioThumb.getTotalLength(), 
        0, 
        1.0f
      );
      //to draw the rectangle to indicate which section the audio is being played at
      g.setColour(Colours::green);
      g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
    }
    else 
    {
      //to display text when no audio is loaded
      g.setFont (20.0f);
      g.drawText ("LOAD A FILE", getLocalBounds(),
                  Justification::centred, true);
    }

    //to draw the red vertical line of the checkpoint system
    g.setColour(Colours::red);
    g.fillRect(checkPointPosition * getWidth(), 0, 2, getHeight());
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::loadURL(URL audioURL)
{
  //to draw up the waveform of the audio file loaded
  audioThumb.clear();
  fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
  if (fileLoaded)
  {
    std::cout << "wfd: loaded! " << std::endl;
    repaint();
  }
  else {
    std::cout << "wfd: not loaded! " << std::endl;
  }

}

void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;
    //to display the waveform after loading an audio
    repaint();

}

void WaveformDisplay::setPositionRelative(double pos)
{
  //to move the green rectangle accordingly when the audio is playing
  if (pos != position)
  {
    position = pos;
    repaint();
  }  
}




