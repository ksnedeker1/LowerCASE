#pragma once

#include "Generator.h"
#include <JuceHeader.h>

class Beeper : public Generator
{
public:
    Beeper();
    virtual ~Beeper() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    const juce::String Beeper::getName() const override;

private:
    juce::dsp::Oscillator<float> oscillator;
    float currentFrequency;
    int sampleCounter;
    int soundDuration;
    int silenceDuration;
};
