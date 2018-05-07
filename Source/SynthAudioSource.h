#pragma once
#include "JuceMaxiOsc.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "PresetData.h"

class SynthAudioSource   : public AudioSource
{
public:
    SynthAudioSource (MidiKeyboardState& keyState, int numVoices, PresetData *presetData);
    
    void setUsingSineWaveSound();
    void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    MidiMessageCollector* getMidiCollector();

private:
    double m_attack = 0.1;
    double m_release = 0.9999;
    long m_holdTime = 1;
    
    MidiKeyboardState& keyboardState;
    Synthesiser synth;
    MidiMessageCollector midiCollector;
};
