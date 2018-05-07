#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Maximilian/maximilian.h"

class ADSRComponent : public Component
{
public:
    ADSRComponent(double *attack, double *decay, double *sustain, double *release, long *holdTime);
    //~ADSRComponent();
    
    void resized() override;
    
private:
    Slider m_attackSlider, m_decaySlider, m_sustainSlider, m_releaseSlider;
    Label  m_attackLabel,  m_decayLabel,  m_sustainLabel,  m_releaseLabel;
};
