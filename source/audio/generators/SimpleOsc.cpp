#include "SimpleOsc.h"

SimpleOsc::SimpleOsc(float frequency, const juce::String& waveform)
    : currentFrequency(frequency)
{
    setWaveform(waveform);
}

SimpleOsc::~SimpleOsc() {}

void SimpleOsc::setWaveform(const juce::String& waveform)
{
    if (waveform == "sine")
        oscillator.initialise([](float x) { return std::sin(x); });
    else if (waveform == "saw")
        oscillator.initialise([](float x) { return x / juce::MathConstants<float>::pi - 1.0f; });
    else if (waveform == "square")
        oscillator.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
    else if (waveform == "triangle")
        oscillator.initialise([](float x) { return 2.0f * std::abs(2.0f * (x / juce::MathConstants<float>::twoPi) - std::floor((x / juce::MathConstants<float>::twoPi) + 0.5f)) - 1.0f; });
    else
        oscillator.initialise([](float x) { return std::sin(x); });
}

void SimpleOsc::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();

    oscillator.prepare(spec);
}

void SimpleOsc::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);
    oscillator.setFrequency(currentFrequency);
    oscillator.process(context);
}

void SimpleOsc::setFrequency(float frequency) { currentFrequency = frequency; }
