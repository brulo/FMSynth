/*
  ==============================================================================

    Synth.h
    Created: 22 Apr 2018 9:39:09pm
    Author:  brulo

  ==============================================================================
*/

#pragma once
#include "JuceMaxiOsc.h"

//==============================================================================
class SynthAudioSource   : public AudioSource
{
public:
    SynthAudioSource (MidiKeyboardState& keyState)
    : keyboardState (keyState)
    {
        for(int i = 0; i < 4; ++i)
        {
            synth.addVoice(new JuceMaxiOscVoice(JuceMaxiOscType::Saw));
        }
        
        synth.addSound(new JuceMaxiOscSound());
    }
    
    void setUsingSineWaveSound()
    {
        synth.clearSounds();
    }
    
    void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        synth.setCurrentPlaybackSampleRate(sampleRate);
        midiCollector.reset(sampleRate);
    }
    
    void releaseResources() override {}
    
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
        
        MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
        
        keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample,
                                            bufferToFill.numSamples, true);
        
        synth.renderNextBlock(*bufferToFill.buffer, incomingMidi,
                              bufferToFill.startSample, bufferToFill.numSamples);
    }
    
    MidiMessageCollector* getMidiCollector()
    {
        return &midiCollector;
    }
    
private:
    MidiKeyboardState& keyboardState;
    Synthesiser synth;
    MidiMessageCollector midiCollector;
};
