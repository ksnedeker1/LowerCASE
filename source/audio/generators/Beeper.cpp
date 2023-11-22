#include "Beeper.h"

Beeper::Beeper()
    : currentFrequency(220.0f),
    sampleCounter(0),
    soundDuration(0),
    silenceDuration(0)
{
    oscillator.initialise([](float x) { return x / juce::MathConstants<float>::pi - 1.0f; });
}

Beeper::~Beeper() {}

void Beeper::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();

    oscillator.prepare(spec);

    // beep for 1/3, silence for 2/3
    soundDuration = static_cast<int>(sampleRate / 3);
    silenceDuration = static_cast<int>(2 * sampleRate / 3);
}

void Beeper::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);

    oscillator.setFrequency(currentFrequency);

    oscillator.process(context);

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            if (sampleCounter >= soundDuration)
            {
                channelData[sample] = 0.0f;
            }
            sampleCounter++;
            if (sampleCounter >= (soundDuration + silenceDuration))
            {
                sampleCounter = 0;
            }
        }
    }
    if (buffer.getNumChannels() > 0)
    {
        auto* channelData = buffer.getReadPointer(0);
        DBG("Buffer Sample Check: ");
        for (int i = 0; i < buffer.getNumSamples() && i < 10; ++i)
        {
            DBG("Beeper sample sent " << i << ": " << channelData[i]);
        }
    }
}

const juce::String Beeper::getName() const { return "Beeper"; }
