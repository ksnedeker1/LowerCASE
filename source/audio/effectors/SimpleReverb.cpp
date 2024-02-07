#include "SimpleReverb.h"

//-----------------------------------------------------------------------------------//

SimpleReverb::SimpleReverb(float roomSize, float damping, float width,
    float wetDry, float freezeMode)
{
    setReverbParameters(roomSize, damping, width, wetDry, freezeMode);
}

//-----------------------------------------------------------------------------------//

SimpleReverb::~SimpleReverb() {}

//-----------------------------------------------------------------------------------//

void SimpleReverb::prepareToPlay(double sampleRate, int samplesPerBlock) 
{
    reverb.setSampleRate(sampleRate);
}

//-----------------------------------------------------------------------------------//

void SimpleReverb::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) 
{
    reverb.setParameters(reverbParameters);

    if (buffer.getNumChannels() >= 2) {
        reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
    }
    else if (buffer.getNumChannels() == 1) {
        reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
    }
}

//-----------------------------------------------------------------------------------//

void SimpleReverb::reset() 
{
    reverb.reset();
}

//-----------------------------------------------------------------------------------//

void SimpleReverb::setReverbParameters(float roomSize, float damping, float width,
    float wetDry, float freezeMode) 
{
    jassert(roomSize >= 0.0f && roomSize <= 1.0f);
    jassert(damping >= 0.0f && damping <= 1.0f);
    jassert(width >= 0.0f && width <= 1.0f);
    jassert(wetDry >= 0.0f && wetDry <= 1.0f);
    jassert(freezeMode == 0.0f || freezeMode == 1.0f);

    reverbParameters.roomSize = roomSize;
    reverbParameters.damping = damping;
    reverbParameters.width = width;
    reverbParameters.wetLevel = wetDry;
    reverbParameters.dryLevel = 1.0f - wetDry;
    reverbParameters.freezeMode = freezeMode;

    reverb.setParameters(reverbParameters);
}

//-----------------------------------------------------------------------------------//

juce::Reverb::Parameters SimpleReverb::getReverbParameters() const 
{
    return reverbParameters;
}

//-----------------------------------------------------------------------------------//

const juce::String SimpleReverb::getName() const { return "SimpleReverb"; }