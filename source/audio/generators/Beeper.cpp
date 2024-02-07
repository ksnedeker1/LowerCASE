#include "Beeper.h"

//-----------------------------------------------------------------------------------//

Beeper::Beeper(float initialFrequency, float soundDurationProportion)
    : currentFrequency(initialFrequency),
    sampleCounter(0),
    soundDuration(0),
    soundDurationProportion(soundDurationProportion)
{
    // Saw wave
    oscillator.initialise([](float x) { return x / juce::MathConstants<float>::pi - 1.0f; });
}

Beeper::~Beeper() {}

//-----------------------------------------------------------------------------------//

void Beeper::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();

    oscillator.prepare(spec);
    oscillator.setFrequency(currentFrequency);

    // Calculate sound and silence durations based on the proportion
    soundDuration = static_cast<int>(sampleRate * soundDurationProportion);
    silenceDuration = static_cast<int>(sampleRate * (1.0f - soundDurationProportion));
}

//-----------------------------------------------------------------------------------//

void Beeper::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);

    oscillator.setFrequency(currentFrequency);

    // Write signal to all samples in buffer
    oscillator.process(context);

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {   
            // Overwrite samples beyond soundDuration with silence
            if (sampleCounter >= soundDuration)
            {
                channelData[sample] = 0.0f;
            }

            sampleCounter++;

            // Reset when cycle length is met
            if (sampleCounter >= (soundDuration + silenceDuration))
            {
                sampleCounter = 0;
            }
        }
    }
}

//-----------------------------------------------------------------------------------//

const juce::String Beeper::getName() const { return "Beeper"; }