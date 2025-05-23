#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"
#include <iostream>
#include <string>
#include <sstream>

DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse,
                int _rC, int _gC, int _bC
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse),
               rC(_rC), gC(_gC), bC(_bC)
{
    //setting up the listeners
    addAndMakeVisible(playNstopButton);
    addAndMakeVisible(repeatButton);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(repeatSlider);

    playNstopButton.addListener(this);
    repeatButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    repeatSlider.addListener(this);

    //changing the UI of the listeners
    volSlider.setRange(0.0, 2.0);
    volSlider.setSliderStyle(juce::Slider::Rotary);
    volSlider.setTextBoxStyle(Slider::TextBoxLeft,true,50,15);
    volSlider.setValue(0.2);
    volSlider.setNumDecimalPlacesToDisplay(2);
    volSlider.setColour(Slider::textBoxOutlineColourId,Colours::limegreen);
    volSlider.setColour(Slider::textBoxBackgroundColourId,Colours::grey);
    volSlider.setColour(Slider::rotarySliderOutlineColourId, juce::Colours::grey);
    volSlider.setColour(Slider::rotarySliderFillColourId, juce::Colours::white);

    speedSlider.setRange(0.0, 3.0);
    speedSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    speedSlider.setTextBoxStyle(Slider::TextBoxLeft, true, 50, 15);
    speedSlider.setValue(1.0);
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setColour(Slider::textBoxOutlineColourId, Colours::limegreen);
    speedSlider.setColour(Slider::textBoxBackgroundColourId, Colours::grey);
    speedSlider.setColour(Slider::backgroundColourId, juce::Colours::grey);
    speedSlider.setColour(Slider::trackColourId, juce::Colours::white);

    posSlider.setRange(0.0, 1.0);
    posSlider.setSliderSnapsToMousePosition(true);
    posSlider.setSliderStyle(juce::Slider::LinearBar);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    posSlider.setColour(juce::Slider::trackColourId, juce::Colours::transparentWhite);

    repeatSlider.setRange(0.0, 1.0);
    repeatSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    repeatSlider.setColour(Slider::thumbColourId, juce::Colours::red);
    repeatSlider.setValue(0.0);

    //To store the CFSlider's value
    CFValue = 0.5;

    //To allow vinyl disk and cue arm to rotate
    startTimer(20);
    handAngle = 0.0;
    audioStateP = true;
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    //set the deck's background color
    g.fillAll(juce::Colour(rC,gC,bC));

    g.setColour (Colours::white);
    g.drawRect(getLocalBounds(), 1); 
    g.setFont (20.0f);

    //to display the loaded song's name and duration
    g.drawText("Title : "+title, 10, 2, getWidth() / 4 - 10, getHeight() / 8, Justification::centredLeft, true);
    g.drawText("Duration : "+songduration, 10, 2 + getHeight() / 8, getWidth() / 4 - 10, getHeight() / 8, Justification::centredLeft, true);

    //to display the checkpoint's time stamp
    g.setFont(14.0f);
    std::string checkpointLine = checkpointPos + "/" + songduration;
    g.drawText("Checkpoint : " + checkpointLine, getWidth() * 3/8 + 10, getHeight() / 3, getWidth() / 3, getHeight() / 6, Justification::centredLeft, true);
 
    //vinyl disk
    g.saveState();
    auto vinylDisk = ImageCache::getFromMemory(BinaryData::vinyldisk_png, BinaryData::vinyldisk_pngSize);
    g.addTransform(juce::AffineTransform::translation(getWidth() / 20, getHeight() /2));
    g.addTransform(juce::AffineTransform::rotation(angle, vinylDisk.getWidth() / 2, vinylDisk.getHeight() / 2));
    g.drawImageAt(vinylDisk, 0, 0, false);
    g.restoreState();

    //cue arm
    g.saveState();
    auto vinylHand = ImageCache::getFromMemory(BinaryData::vinylHand_png, BinaryData::vinylHand_pngSize);
    g.addTransform(juce::AffineTransform::translation(getWidth() / 20  - vinylDisk.getWidth()/1.25, getHeight() / 2));
    if (player->isPlaying()) {
        if (handAngle < 0.5) {
            handAngle = handAngle + 0.1;;
        }
    }
    else {
        if (handAngle > 0.1) {
            handAngle = handAngle - 0.1;
        }
    }
    g.addTransform(juce::AffineTransform::rotation(handAngle, vinylHand.getWidth() / 2, vinylHand.getHeight() / 2));
    g.drawImageAt(vinylHand, 0, 0, false);
    g.restoreState();
}

void DeckGUI::resized()
{
    playNstopButton.setBounds(getWidth() / 4, 0, getWidth() / 8, getHeight() / 3);
    repeatButton.setBounds(getWidth() / 4, getHeight()/3, getWidth() / 8, getHeight() / 6);

    volSlider.setBounds(getWidth()*5/8, 0, getWidth()/4, getHeight()*2/3);
    speedSlider.setBounds(getWidth()*7/8, 0, getWidth()/8, getHeight()*2/3);
    
    waveformDisplay.setBounds(getWidth()/4, getHeight()*2/3, getWidth()*3/4, getHeight()/3);
    posSlider.setBounds(getWidth() / 4, getHeight() * 2 / 3, getWidth()*3/4, getHeight() / 3);

    repeatSlider.setBounds(getWidth() / 4, getHeight() * 2 / 3 - 10, getWidth() * 3 / 4, 10);
}


void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playNstopButton)
    {
        if (audioStateP) {
            player->start();
            audioStateP = false;
            button->setButtonText("STOP");
        }
        else {
            player->stop();
            audioStateP = true;
            button->setButtonText("PLAY");
            //setButtonText() allows the button to display different text after each interaction
        }
    }
    if (button == &repeatButton)
    {
        double RPS = repeatSlider.getValue();
        if (RPS >= 0 && RPS <= 1) { //ensuring the slider value is inside an acceptable range
            double songTotalLength = player->getLengthInSeconds();
            double location = songTotalLength * RPS;
            player->setPositionRelative(location);
        }

    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue() * CFValue);
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        if (slider->getValue() >= 0 && slider->getValue() <= 1) {
            double songTotalLength = player->getLengthInSeconds();
            double location = songTotalLength * slider->getValue();
            player->setPositionRelative(location);
        }
    }
    if (slider == &repeatSlider)
    {
        //to allow the red vertical line of the checkpoint system to display itself at the right position
        waveformDisplay.checkPointPosition = slider->getValue();
        if (songduration != " ") {
            //songduration is a string var that contain string in the format of "hh:mm:ss"
            //it is converted into seconds in the format of int s.
            int songdurationinSec = convertToSeconds(songduration);
            double relativeTimeinSec = songdurationinSec * slider->getValue();
            double relativeTimeinMin = relativeTimeinSec / 60;
            double relativeTimeinHr = relativeTimeinSec / 3600;
            //to get the remainder seconds
            int seconds = (relativeTimeinMin - floor(relativeTimeinMin)) * 60;
            //to get the remainder seconds in term of minutes
            int minutes = (relativeTimeinHr - floor(relativeTimeinHr)) * 60;
            //to get the hour
            int hours = floor(relativeTimeinHr);

            //to convert seconds,minutes, and hours into string
            std::string hourString = std::to_string(hours);
            std::string minString = std::to_string(minutes);
            std::string secString = std::to_string(seconds);

            //add 0 to the front of seconds,minutes, or hours if they are lesser than 10
            if (hours < 10) {
                hourString = "0" + hourString;
            }
            if (minutes < 10) {
                minString = "0" + minString;
            }
            if (seconds < 10) {
                secString = "0" + secString;
            }

            //compiling the time into proper string format.
            checkpointPos = hourString + ":" + minString + ":" + secString;
        }
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  //only load one song when dragged onto the deck
  if (files.size() == 1){
      auto songURL = URL{ File{files[0]} };
      player->loadURL(songURL);
      waveformDisplay.loadURL(songURL);
      //remove the directory of the input, leaving on the filename and its format
      title = juce::URL::removeEscapeChars(songURL.getFileName());
      //get the audio's time in the format of "hh:mm:ss"
      songduration = getSongsTime(File{ files[0] });
  }
}

void DeckGUI::timerCallback()
{
    //if an audio is playing, rotate the disk
    if (player->isPlaying())
    {
        //this allow the vinyl disk to increase its rotate speed according to the audio's speed
        angle += 0.05 * speedSlider.getValue();
    }
    
    //when the angle exceed 2PI, reset it back to 0
    if (angle >= 2 * juce::MathConstants<float>::pi) {
        angle -= 2 * juce::MathConstants<float>::pi;
    }

    //allow the changes to the vinyl disk and the cue arm to be displayed
    repaint();
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}


int DeckGUI::convertToSeconds(const std::string& timeStr) {
    int hours, minutes, seconds;
    char colon;
    //to convert string into time format
    std::istringstream timeInStream(timeStr);

    //allow the variables to know what to takes based on the format of "hh:mm:ss"
    timeInStream >> hours >> colon >> minutes >> colon >> seconds;

    //convert it into seconds, and then return the output
    return hours * 3600 + minutes * 60 + seconds;
}

//get an audio's duration in string format
std::string DeckGUI::getSongsTime(const juce::File chosenFile) {
    formatManager.registerBasicFormats();
    auto reader = formatManager.createReaderFor(chosenFile);
    //essentially, finding how many seconds it takes to play the audio fully
    auto duration = reader->lengthInSamples / reader->sampleRate;

    //to convert it into human calender time format
    std::time_t epochTime(duration);
    tm* calenderTime = gmtime(&epochTime);

    std::string hours = std::to_string(calenderTime->tm_hour);
    std::string minutes = std::to_string(calenderTime->tm_min);
    std::string seconds = std::to_string(calenderTime->tm_sec);

    if (calenderTime->tm_hour < 10) {
        hours = "0" + hours;
    }

    if (calenderTime->tm_min < 10) {
        minutes = "0" + minutes;
    }

    if (calenderTime->tm_sec < 10) {
        seconds = "0" + seconds;
    }

    //to compile into string format of "hh:mm:ss"
    std::string songTime = hours + ":" + minutes + ":" + seconds;

    return songTime;
}