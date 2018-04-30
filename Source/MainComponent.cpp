#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent() : synthAudioSource  (keyboardState),
                                 keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard),
                                 filterAudioSource (&synthAudioSource, false)
{
    // filter cutoff freq slider
    addAndMakeVisible (filterCutoffFreqSlider);
    filterCutoffFreqSlider.setRange (20.0, 20000.0);
    filterCutoffFreqSlider.setTextValueSuffix (" Hz");
    filterCutoffFreqSlider.onValueChange = [this]
    {
        IIRCoefficients coef = IIRCoefficients::makeLowPass(sampleRate, filterCutoffFreqSlider.getValue());
        filterAudioSource.setCoefficients(coef);
    };
    
    addAndMakeVisible (midiInputListLabel);
    midiInputListLabel.setText ("MIDI Input:", dontSendNotification);
    midiInputListLabel.attachToComponent (&midiInputList, true);
    
    auto midiInputs = MidiInput::getDevices();
    addAndMakeVisible (midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
    midiInputList.addItemList (midiInputs, 1);
    midiInputList.onChange = [this] { setMidiInput (midiInputList.getSelectedItemIndex()); };
    
    for(auto midiInput : midiInputs)
    {
        if(deviceManager.isMidiInputEnabled (midiInput))
        {
            setMidiInput(midiInputs.indexOf (midiInput));
            break;
        }
    }
    
    if(midiInputList.getSelectedId() == 0)
        setMidiInput(0);
    
    addAndMakeVisible(keyboardComponent);
    setAudioChannels(0, 2);
    
    setSize (600, 190);
    startTimer (400);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    midiInputList    .setBounds (200, 10, getWidth() - 210, 20);
    keyboardComponent.setBounds (10,  40, getWidth() - 20, getHeight() - 50);
    filterCutoffFreqSlider.setBounds(10, 0, 200, 50);
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    filterAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    this->sampleRate = sampleRate;
    filterAudioSource.setCoefficients(IIRCoefficients::makeLowPass(sampleRate, 20000));
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    filterAudioSource.getNextAudioBlock (bufferToFill);
}

void MainComponent::releaseResources()
{
    filterAudioSource.releaseResources();
}

void MainComponent::timerCallback()
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}

void MainComponent::setMidiInput (int index)
{
    auto list = MidiInput::getDevices();
    
    deviceManager.removeMidiInputCallback (list[lastInputIndex], synthAudioSource.getMidiCollector());
    
    auto newInput = list[index];
    
    if (! deviceManager.isMidiInputEnabled (newInput))
        deviceManager.setMidiInputEnabled (newInput, true);
    
    deviceManager.addMidiInputCallback (newInput, synthAudioSource.getMidiCollector());
    midiInputList.setSelectedId (index + 1, dontSendNotification);
    
    lastInputIndex = index;
}

