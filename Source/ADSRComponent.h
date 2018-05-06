#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class ADSRComponent : public Component
{
public:
    ADSRComponent();
    //~ADSRComponent();
    
    void resized() override;
    
    double m_attack = 0.1;
    double m_decay = 0.1;
    double m_sustain = 0.5;
    double m_release = 0.999;
    long m_holdTime = 1;
    
private:
    Slider m_attackSlider, m_decaySlider, m_sustainSlider, m_releaseSlider;
    Label  m_attackLabel,  m_decayLabel,  m_sustainLabel,  m_releaseLabel;
};
