#pragma once

#include <JuceHeader.h>

/*------------------------------------------------------------------------------------\
| Parent class for Generator and Effector module types
\------------------------------------------------------------------------------------*/
class BaseProcessor : public juce::AudioProcessor
{
public:
    //-----------------------------------------------------------------------------------//

    BaseProcessor();
    ~BaseProcessor() override;

    //-----------------------------------------------------------------------------------//

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    //-----------------------------------------------------------------------------------//

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void reset() override;

    //-----------------------------------------------------------------------------------//

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //-----------------------------------------------------------------------------------//

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //-----------------------------------------------------------------------------------//
    
    const juce::String getName() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
};
