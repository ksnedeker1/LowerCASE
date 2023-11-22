#pragma once

#include <JuceHeader.h>
#include "Audio.h"
#include "MixerController.h"
#include "ChainController.h"
#include "AudioGraphWrapper.h"

//==============================================================================
class MainComponent  : public juce::AudioAppComponent, private juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //============================================================================== 
    void timerCallback() override;
    void addSecondChain();

    Chain audioChain;
    juce::AudioProcessorGraph mixerGraph;
    juce::AudioProcessorGraph audioGraph;

    ChainController chainController;
    MixerController mixerController;

    juce::AudioProcessorGraph::Node::Ptr simpleOscNode;
    juce::AudioProcessorGraph::Node::Ptr beeperNode;
    juce::AudioProcessorGraph::Node::Ptr delayNode;
    juce::AudioProcessorGraph::Node::Ptr simpleReverbNode;
    juce::AudioProcessorGraph::Node::Ptr gainNode;

    juce::AudioProcessorGraph::Node::Ptr outputNode;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
