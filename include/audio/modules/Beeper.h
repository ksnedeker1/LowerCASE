#pragma once

#include "Generator.h"
#include <JuceHeader.h>

/*------------------------------------------------------------------------------------\
| Emits a constant beep
\------------------------------------------------------------------------------------*/
class Beeper : public Generator
{
public:
    //-----------------------------------------------------------------------------------//

    Beeper(float initialFrequency = 220.0f, float soundDurationProportion = 0.3333f);
    virtual ~Beeper() override;

    //-----------------------------------------------------------------------------------//

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    //-----------------------------------------------------------------------------------//

    const juce::String Beeper::getName() const override;

private:
    //-----------------------------------------------------------------------------------//
    // Signal Generation
    
    juce::dsp::Oscillator<float> oscillator;

    //-----------------------------------------------------------------------------------//
    // State

    float currentFrequency;
    float soundDurationProportion;
    int sampleCounter;
    int soundDuration;
    int silenceDuration;
};