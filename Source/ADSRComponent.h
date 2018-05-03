#pragma once

#include "JuceMaxiOsc.h"
#include "../JuceLibraryCode/JuceHeader.h"

class ADSRComponent : public Component
{
public:
    double m_attack = 0.1;
    double m_release = 0.999;
    long m_holdTime = 1;
    
    ADSRComponent()
    {
        const Slider::SliderStyle sliderStyle = Slider::SliderStyle::LinearVertical;
        const int textBoxWidth = 50;
        const int textBoxHeight = 20;
        bool showValueText = false;
        
        // attack label
        addAndMakeVisible(m_attackLabel);
        m_attackLabel.setText("A", NotificationType::dontSendNotification);
        m_attackLabel.setJustificationType(Justification::centred);
        
        // attack slider
        addAndMakeVisible(m_attackSlider);
        m_attackSlider.setRange(1, 1000.0);
        m_attackSlider.setTextValueSuffix(" Ms");
        m_attackSlider.setNumDecimalPlacesToDisplay(0);
        m_attackSlider.setName("A");
        m_attackSlider.setSliderStyle(sliderStyle);
        m_attackSlider.setTextBoxStyle(showValueText ? Slider::TextBoxBelow : Slider::NoTextBox,
                                       true, textBoxWidth, textBoxHeight);
        m_attackSlider.onValueChange = [this]
        { m_attack = 1-pow( 0.01, 1.0 / ( m_attackSlider.getValue() * maxiSettings::sampleRate * 0.001 ) ); };
        
        // decay label
        addAndMakeVisible(m_decayLabel);
        m_decayLabel.setText("D", NotificationType::dontSendNotification);
        m_decayLabel.setJustificationType(Justification::centred);
        
        // decay slider
        addAndMakeVisible(m_decaySlider);
        m_decaySlider.setRange(1.0, 1000.0);
        m_decaySlider.setTextValueSuffix(" Ms");
        m_decaySlider.setNumDecimalPlacesToDisplay(0);
        m_decaySlider.setName("D");
        m_decaySlider.setSliderStyle(sliderStyle);
        m_decaySlider.setTextBoxStyle(showValueText ? Slider::TextBoxBelow : Slider::NoTextBox,
                                      true, textBoxWidth, textBoxHeight);
        //m_decaySlider.onValueChange = [this] { m_decay = m_decaySlier.getValue(); };
        
        // sustain label
        addAndMakeVisible(m_sustainLabel);
        m_sustainLabel.setText("S", NotificationType::dontSendNotification);
        m_sustainLabel.setJustificationType(Justification::centred);
        
        // sustain slider
        addAndMakeVisible(m_sustainSlider);
        m_sustainSlider.setRange(1.0, 1000.0);
        m_sustainSlider.setTextValueSuffix(" Ms");
        m_sustainSlider.setNumDecimalPlacesToDisplay(0);
        m_sustainSlider.setName("S");
        m_sustainSlider.setSliderStyle(sliderStyle);
        m_sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, textBoxWidth, textBoxHeight);
        m_sustainSlider.setTextBoxStyle(showValueText ? Slider::TextBoxBelow : Slider::NoTextBox,
                                        true, textBoxWidth, textBoxHeight);
        //m_sustainSlider.onValueChange = [this]{ m_env.setSustain(m_sustainSlider.getValue()); };
        
        // release label
        addAndMakeVisible(m_releaseLabel);
        m_releaseLabel.setText("R", NotificationType::dontSendNotification);
        m_releaseLabel.setJustificationType(Justification::centred);
        
        // release slider
        addAndMakeVisible(m_releaseSlider);
        m_releaseSlider.setRange(1.0, 1000.0);
        m_releaseSlider.setTextValueSuffix(" Ms");
        m_releaseSlider.setNumDecimalPlacesToDisplay(0);
        m_releaseSlider.setName("R");
        m_releaseSlider.setSliderStyle(sliderStyle);
        m_releaseSlider.setTextBoxStyle(showValueText ? Slider::TextBoxBelow : Slider::NoTextBox,
                                        true, textBoxWidth, textBoxHeight);
        m_releaseSlider.onValueChange = [this]
        { m_release = pow( 0.01, 1.0/(m_releaseSlider.getValue() * maxiSettings::sampleRate * 0.001) ); };
    }
    
    void resized() override
    {
        int sliderWidth = (int)(getWidth() * 0.25f);
        auto boundsRect = getLocalBounds();
        auto labelSlice = boundsRect.removeFromTop(20);
        m_attackLabel.setBounds(labelSlice.removeFromLeft(sliderWidth));
        m_decayLabel.setBounds(labelSlice.removeFromLeft(sliderWidth));
        m_sustainLabel.setBounds(labelSlice.removeFromLeft(sliderWidth));
        m_releaseLabel.setBounds(labelSlice);
        
        m_attackSlider.setBounds(boundsRect.removeFromLeft(sliderWidth));
        m_decaySlider.setBounds(boundsRect.removeFromLeft(sliderWidth));
        m_sustainSlider.setBounds(boundsRect.removeFromLeft(sliderWidth));
        m_releaseSlider.setBounds(boundsRect);
    }

private:
    Slider m_attackSlider, m_decaySlider, m_sustainSlider, m_releaseSlider;
    Label  m_attackLabel,  m_decayLabel,  m_sustainLabel,  m_releaseLabel;
};
