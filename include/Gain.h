#pragma once

#include "Effector.h"

class Gain : public Effector
{
public:
    explicit Gain(float initialGain = 1.0f);
    virtual ~Gain() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void releaseResources() override;

    void setGain(float newGain);
    float getGain() const;

private:
    float gain;
};
