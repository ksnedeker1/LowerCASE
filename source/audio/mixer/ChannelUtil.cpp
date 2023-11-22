#include "ChannelUtil.h"

ChannelUtil::ChannelUtil(float initialGain, float initialPanning, bool initialBypassed)
    : gain(initialGain), panning(initialPanning), bypassed(initialBypassed)
{
}

ChannelUtil::~ChannelUtil() {}

void ChannelUtil::prepareToPlay(double sampleRate, int samplesPerBlock) { }

void ChannelUtil::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (bypassed) { return; }

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Improve panning...
            channelData[sample] *= gain * (channel == 0 ? (1.0f - panning) : panning);
        }
    }
}

void ChannelUtil::reset() {}

void ChannelUtil::setGain(float newGain) { gain = newGain; }

float ChannelUtil::getGain() const { return gain; }

void ChannelUtil::setPanning(float newPanning) { panning = newPanning; }

float ChannelUtil::getPanning() const { return panning; }

void ChannelUtil::setBypassed(bool shouldBeBypassed) { bypassed = shouldBeBypassed; }

bool ChannelUtil::isBypassed() const { return bypassed; }
