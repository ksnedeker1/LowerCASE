#pragma once

#include "Effector.h"
#include <JuceHeader.h>

/*------------------------------------------------------------------------------------\
| Modifies signal gain, panning, and enables bypassing
|
| A Mixer component module. Default state of bypassed should be false if used 
| elsewhere.
\------------------------------------------------------------------------------------*/
class ChannelUtil : public Effector
{
public:
    //-----------------------------------------------------------------------------------//

    ChannelUtil(float gain = 1.0f, float panning = 0.5f, bool bypassed = true);
    virtual ~ChannelUtil() override;

    //-----------------------------------------------------------------------------------//

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void reset() override;

    //-----------------------------------------------------------------------------------//

    void setGain(float newGain);
    float getGain() const;

    void setPanning(float newPanning);
    float getPanning() const;

    void setBypassed(bool shouldBeBypassed);
    bool isBypassed() const;

private:
    //-----------------------------------------------------------------------------------//
    // State

    float gain;
    float panning;
    bool bypassed;
};
