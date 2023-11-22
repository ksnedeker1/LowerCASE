#pragma once

#include "Effector.h"
#include <JuceHeader.h>

class SimpleReverb : public Effector
{
public:
    SimpleReverb(float roomSize = 0.7f, float damping = 0.5f, float width = 1.0f,
        float wetDry = 0.3f, float freezeMode = 0.0f);
    virtual ~SimpleReverb() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void reset() override;

    void setReverbParameters(float roomSize, float damping, float width,
        float wetDry, float freezeMode);
    juce::Reverb::Parameters getReverbParameters() const;

    const juce::String SimpleReverb::getName() const override;

private:
    juce::Reverb reverb;
    juce::Reverb::Parameters reverbParameters;
};
