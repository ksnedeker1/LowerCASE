#pragma once

#include "Effector.h"
#include <JuceHeader.h>

class ChannelUtil : public Effector
{
public:
    ChannelUtil(float gain = 1.0f, float panning = 0.5f, bool bypassed = true);
    virtual ~ChannelUtil() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void reset() override;

    void setGain(float newGain);
    float getGain() const;

    void setPanning(float newPanning);
    float getPanning() const;

    void setBypassed(bool shouldBeBypassed);
    bool isBypassed() const;

private:
    float gain;
    float panning;
    bool bypassed;
};
