#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent() : m_presetData(),
                                 synthAudioSource  (keyboardState, 4, &m_presetData),
                                 keyboardComponent (keyboardState,
                                                    MidiKeyboardComponent::horizontalKeyboard),
                                 filterAudioSource (&synthAudioSource, false),
                                 m_adsrComponent(&m_presetData.attack, &m_presetData.decay, &m_presetData.sustain,
                                                 &m_presetData.release, &m_presetData.holdTime)
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
    
    // envelope
    addAndMakeVisible(m_adsrComponent);
    
    // midi input list
    addAndMakeVisible (midiInputListLabel);
    midiInputListLabel.setText ("MIDI Input:", dontSendNotification);
    midiInputListLabel.attachToComponent (&midiInputList, true);
    
    auto midiInputs = MidiInput::getDevices();
    addAndMakeVisible (midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
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
    
    setSize(640,480);
    
    addAndMakeVisible(keyboardComponent);
    setAudioChannels(0, 2);

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
    auto rect = getLocalBounds();

    // margins
    const int topBottomMargin = 10;
    rect.removeFromTop(topBottomMargin);
    rect.removeFromBottom(topBottomMargin);
    const int leftRightMargin = 10;
    rect.removeFromLeft(leftRightMargin);
    rect.removeFromRight(leftRightMargin);
    
    // midi in
    auto midiInRect = rect.removeFromTop(20);
    midiInputListLabel.setBounds(midiInRect.removeFromLeft(80));
    midiInputList.setBounds(midiInRect);
    
    // keyboard
    keyboardComponent.setBounds(rect.removeFromTop(50));
    
    // test envelope
    auto slice = rect.removeFromTop(100);
    m_adsrComponent.setBounds(slice.removeFromRight((int)(slice.getWidth() * 0.25f)));
    
    // filter cutoff
    filterCutoffFreqSlider.setBounds(slice.removeFromRight((int)slice.getWidth() * 0.25f));
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    filterAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    this->sampleRate = sampleRate;
    maxiSettings::sampleRate = sampleRate;
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

