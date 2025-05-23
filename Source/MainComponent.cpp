#include "MainComponent.h"

MainComponent::MainComponent()
{
    //to define the size of the DJ application
    setSize (800, 600);

    //some require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        //to specify the number of input and output channels
        setAudioChannels (0, 2);
    }  

    //allow the components to be visible
    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);

    addAndMakeVisible(crossFader);
    
    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    //to shut down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    //to be called when the audio device stops
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour(Colours::white);
    g.setFont(14.0f);    
}

void MainComponent::resized()
{
    //to define the components' position
    crossFader.setBounds(0, 0, getWidth() / 20, getHeight() *  9/ 12);
    deckGUI1.setBounds(getWidth()/20, 0, getWidth() * 19/20, getHeight() * 4.5/12);
    deckGUI2.setBounds(getWidth()/20, getHeight()*4.5/12, getWidth()*19/20 , getHeight() *  4.5/12);
    playlistComponent.setBounds(0, getHeight()*9/12, getWidth(), getHeight()*3/12);
}
