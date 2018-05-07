#include "FilterComponent.h"

FilterComponent::FilterComponent(double *cutoff)
{
    const Slider::SliderStyle sliderStyle = Slider::SliderStyle::LinearVertical;
    const int textBoxWidth = 50;
    const int textBoxHeight = 20;
    const bool showValueText = false;
    
    // attack label
    addAndMakeVisible(m_cutoffLabel);
    m_cutoffLabel.setText("Cutoff", NotificationType::dontSendNotification);
    m_cutoffLabel.setJustificationType(Justification::centred);
    
    // attack slider
    addAndMakeVisible(m_cutoffSlider);
    m_cutoffSlider.setRange(0.0, 1.0);
    //m_cutoffSlider.setTextValueSuffix("");
    m_cutoffSlider.setNumDecimalPlacesToDisplay(2);
    m_cutoffSlider.setName("Cutoff");
    m_cutoffSlider.setSliderStyle(sliderStyle);
    m_cutoffSlider.setTextBoxStyle(showValueText ? Slider::TextBoxBelow : Slider::NoTextBox,
                                   true, textBoxWidth, textBoxHeight);
    m_cutoffSlider.onValueChange = [this,cutoff] () mutable { *cutoff = m_cutoffSlider.getValue(); };
    m_cutoffSlider.setValue(1.0);
}

void FilterComponent::resized()
{
    auto boundsRect = getLocalBounds();
    m_cutoffLabel.setBounds(boundsRect.removeFromTop(20));
    m_cutoffSlider.setBounds(boundsRect);
}
