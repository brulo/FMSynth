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
    JuceMaxiOscVoice(JuceMaxiOscType oscType, double *attack, double *decay, double *sustain, double *release, long *holdTime)
    {
        m_oscType = oscType;
        m_attack = attack;
        m_decay = decay;
        m_sustain = sustain;
        m_release = release;
        m_holdTime = holdTime;

        m_trigger = false;
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
        m_trigger = 1;
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
            
            // low pass filtering
            currentSample = m_filt.lopass(currentSample, 0.2);
            
            // amp envelope
            currentSample = m_env.adsr(currentSample, *m_attack, *m_decay, *m_sustain, *m_release, *m_holdTime, m_trigger);
            
            if(m_trigger != 0) m_trigger = 0;

            for(int channelIndex = outputBuffer.getNumChannels(); --channelIndex >= 0;)
            {
                outputBuffer.addSample(channelIndex, startSample, currentSample);
            }
            
            ++startSample;
        }
    }
    
private:
    double *m_attack;
    double *m_decay;
    double *m_sustain;
    double *m_release;
    long* m_holdTime;
    maxiOsc m_osc;
    maxiEnv m_env;
    maxiFilter m_filt;
    JuceMaxiOscType m_oscType;
    double m_frequency;
    int m_trigger;
};
