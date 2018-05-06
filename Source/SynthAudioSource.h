#pragma once
#include "JuceMaxiOsc.h"
#include "../JuceLibraryCode/JuceHeader.h"

class SynthAudioSource   : public AudioSource
{
public:
    SynthAudioSource (MidiKeyboardState& keyState, int numVoices,
                      double *attack, double *decay, double *sustain, double *release, long *holdTime);
    
    void setUsingSineWaveSound();
    void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    MidiMessageCollector* getMidiCollector();

private:
    MidiKeyboardState& keyboardState;
    Synthesiser synth;
    MidiMessageCollector midiCollector;
    double m_attack = 0.1;
    double m_release = 0.9999;
    long m_holdTime = 1;
};
