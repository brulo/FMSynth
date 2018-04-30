#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthAudioSource.h"

class MainComponent   : public AudioAppComponent,
                        private Timer
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
    SynthAudioSource synthAudioSource;
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;
    IIRFilterAudioSource filterAudioSource;
    
    Slider filterCutoffFreqSlider;
    
    double sampleRate;
    
    ComboBox midiInputList;
    Label midiInputListLabel;
    int lastInputIndex = 0;
    
    void setMidiInput(int index);
    void timerCallback() override;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
