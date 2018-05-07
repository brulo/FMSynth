#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class FilterComponent : public Component
{
public:
    FilterComponent(double *cutoff);
    //~ADSRComponent();
    
    void resized() override;
    
private:
    Slider m_cutoffSlider;
    Label  m_cutoffLabel;
};
