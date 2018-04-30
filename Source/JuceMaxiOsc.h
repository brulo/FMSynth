#pragma once

#include "Maximilian/maximilian.h"

enum JuceMaxiOscType
{
    Tri,
    Sin,
    Saw,
    Square,
    //Noise,
};

struct JuceMaxiOscSound : public SynthesiserSound
{
    JuceMaxiOscSound() {}
    
    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
};

struct JuceMaxiOscVoice : public SynthesiserVoice
{
    JuceMaxiOscVoice(JuceMaxiOscType oscType)
    {
        m_oscType = oscType;
    }
    
    bool canPlaySound(SynthesiserSound* sound) override
    {
        return dynamic_cast<JuceMaxiOscSound*> (sound) != nullptr;
    }
    
    void startNote(int midiNoteNumber, float velocity,
                    SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        m_osc.phaseReset(0.0);
        m_frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    }
    
    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        m_osc.phaseReset(0.0);
    }
    
    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}
    
    void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        while(--numSamples >= 0)
        {
            float currentSample;
            switch(m_oscType)
            {
                case JuceMaxiOscType::Saw:
                    currentSample = (float)m_osc.saw(m_frequency);
                    break;
                    
                case JuceMaxiOscType::Square:
                    currentSample = (float)m_osc.square(m_frequency);
                    break;
                    
                case JuceMaxiOscType::Sin:
                    currentSample = (float)m_osc.sinewave(m_frequency);
                    break;
                    
                case JuceMaxiOscType::Tri:
                    currentSample = (float)m_osc.triangle(m_frequency);
                    break;
            }
            
            for(int channelIndex = outputBuffer.getNumChannels(); --channelIndex >= 0;)
            {
                outputBuffer.addSample(channelIndex, startSample, currentSample);
            }
            
            ++startSample;
        }
    }
    
private:
    maxiOsc m_osc;
    JuceMaxiOscType m_oscType;
    double m_frequency;
};
