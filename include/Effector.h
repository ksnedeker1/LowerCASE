#pragma once

#include "BaseProcessor.h"

class Effector : public BaseProcessor
{
public:
    Effector();
    virtual ~Effector() override;

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    const juce::String Effector::getName() const override;
};