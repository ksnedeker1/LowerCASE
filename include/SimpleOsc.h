#pragma once

#include "Generator.h"
#include <JuceHeader.h>

class SimpleOsc : public Generator
{
public:
    SimpleOsc(float frequency = 440.0f, const juce::String& waveform = "sine");
    virtual ~SimpleOsc() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    void setFrequency(float frequency);
    void setWaveform(const juce::String& waveform);

private:
    juce::dsp::Oscillator<float> oscillator;
    float currentFrequency;
};
