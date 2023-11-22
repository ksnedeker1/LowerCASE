#include "Gain.h"

Gain::Gain(float initialGain) : gain(initialGain)
{
    jassert(initialGain >= 0.0f && initialGain <= 1.0f);
}

Gain::~Gain() {}

void Gain::prepareToPlay(double sampleRate, int samplesPerBlock) {}

void Gain::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) { buffer.applyGain(gain); }

void Gain::releaseResources() {}

void Gain::setGain(float newGain)
{
    jassert(newGain >= 0.0f && newGain <= 1.0f);
    gain = newGain;
}

float Gain::getGain() const { return gain; }
