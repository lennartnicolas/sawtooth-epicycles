#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AnimatedAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void update() override;
    juce::Rectangle<float> getAreaForCircle(float radius, const juce::Point<float>& position);
    void updateCircleNumber(int cNumber);

private:
    //==============================================================================
    juce::Point<float> origin;
    juce::Point<float> midPoint;

    std::vector<juce::Rectangle<float> > circleAreas;
    std::vector<juce::Line<float> > circleLines;
    int circleNumber;
    
    float x;
    float y;
    float prevx;
    float prevy;
    float angle;
    float frequency;
    int sampleRate;
    
    std::deque<float> wave;

    juce::Slider circleSlider;
    juce::Slider freqSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
