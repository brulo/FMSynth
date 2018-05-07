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
    MidiKeyboardState& keyboardState;
    Synthesiser synth;
    MidiMessageCollector midiCollector;
};
