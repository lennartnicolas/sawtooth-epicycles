#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(circleSlider);
    addAndMakeVisible(freqSlider);

    circleSlider.setValue(5);
    circleSlider.setRange(1, 100, 1);
    circleSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    circleSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 50, 20);
    circleSlider.setSkewFactorFromMidPoint(15);
    circleSlider.onValueChange = [this](){
        updateCircleNumber((int)circleSlider.getValue());
    };
    
    freqSlider.setValue(0.1);
    freqSlider.setRange(0.01, 1, 0.01);
    freqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    freqSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 50, 20);
    freqSlider.setSkewFactorFromMidPoint(0.4);
    freqSlider.onValueChange = [this](){
        frequency = freqSlider.getValue();
    };
    
    sampleRate = 30;
    setFramesPerSecond(sampleRate);
    
    setSize (900, 500);
    origin = juce::Point<float>((float)(getWidth()/4.f), (float)(getHeight()/2));
    midPoint = juce::Point<float>((float)getWidth()/2,(float)getHeight()/2);
    
    radius = 75;
    circleNumber = 10;
    updateCircleNumber(circleNumber);
 
}

MainComponent::~MainComponent()
{
}

void MainComponent::update()
{
    x = origin.x;
    y = origin.y;
    
    for(int i = 0; i < circleNumber; i++){
        
        prevx = x;
        prevy = y;
        angle = -1 * (float) getFrameCounter() / sampleRate * juce::MathConstants<float>::twoPi * frequency;
        
        int n = i + 1;
        if (n % 2 != 0) {n = -n;}
        
        float r = (radius * 4) / ((float)n * juce::MathConstants<float>::pi);
        x += r * std::cos((i + 1) * angle);
        y += r * std::sin((i + 1)* angle);
        
        circleAreas[i].setSize(std::abs(r * 2), std::abs(r * 2));
        circleAreas[i].setCentre(prevx, prevy);
        circleLines[i].setStart(prevx, prevy);
        circleLines[i].setEnd(x, y);
    }
    
    wave.push_front(y);
    
    if(wave.size() >= getWidth()/2) wave.pop_back();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    
    g.setOpacity(0.3);
    for(const auto& area : circleAreas){

        g.drawEllipse(area, 1.f);
    }
    
    g.setOpacity(1.0);
    for(const auto& line : circleLines)
        g.drawLine(line);
    
    for(int i = 1; i < wave.size(); i++)
        g.drawLine(midPoint.x + i - 1, wave[i - 1], midPoint.x + i, wave[i]);
    
    g.drawLine(x, y, midPoint.x, y);
    g.setColour(juce::Colours::yellow);
    g.drawLine(origin.x, origin.y, circleLines[circleLines.size()-1].getEnd().getX(), circleLines[circleLines.size()-1].getEnd().getY(), 3.f);
    
}

void MainComponent::resized()
{
    circleSlider.setBounds(getWidth()/4 - 100, getHeight() - getHeight()/4 + 30, 200, 30);
    freqSlider.setBounds(getWidth()/4 - 100, getHeight() - getHeight()/4 + 60, 200, 30);
}

juce::Rectangle<float> MainComponent::getAreaForCircle(float radius, const juce::Point<float>& position)
{
    juce::Rectangle<float> area;
    area.setSize(radius * 2, radius * 2);
    area.setCentre(position);
    return area;
}

void MainComponent::updateCircleNumber(int cNumber)
{
    circleNumber = cNumber;
    circleAreas.clear();
    circleLines.clear();
    
    for(int i = 0; i < circleNumber; i++){
        circleAreas.push_back(getAreaForCircle(0.0f, juce::Point<float> (0,0)));
        circleLines.push_back(juce::Line<float>(0,0,0,0));
    }
}
