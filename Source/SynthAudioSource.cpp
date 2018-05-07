#include "SynthAudioSource.h"

SynthAudioSource::SynthAudioSource (MidiKeyboardState& keyState, int numVoices, PresetData *presetData)
: keyboardState (keyState)
{
    std::cout << "m_presetData.attack: " << presetData->attack << std::endl;
    for(int i = 0; i < numVoices; ++i)
    {
        synth.addVoice(new JuceMaxiOscVoice(JuceMaxiOscType::Saw, &(presetData->attack), &(presetData->decay),
                                            &(presetData->sustain), &(presetData->release), &(presetData->holdTime)));
    }
    synth.addSound(new JuceMaxiOscSound());
}
    
void SynthAudioSource::setUsingSineWaveSound()
{
    synth.clearSounds();
}
    
void SynthAudioSource::prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    midiCollector.reset(sampleRate);
}
    
void SynthAudioSource::releaseResources()
{
    
}
    
void SynthAudioSource::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
        
    MidiBuffer incomingMidi;
    midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
        
    keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample,
                                        bufferToFill.numSamples, true);
        
    synth.renderNextBlock(*bufferToFill.buffer, incomingMidi,
                            bufferToFill.startSample, bufferToFill.numSamples);
}
    
MidiMessageCollector* SynthAudioSource::getMidiCollector()
{
    return &midiCollector;
}
