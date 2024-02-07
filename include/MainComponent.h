#pragma once

#include <JuceHeader.h>
#include "Audio.h"
#include "MixerController.h"
#include "ChainController.h"
#include "AudioGraphWrapper.h"
#include "Dispatcher.h"
#include "ActionHandler.h"
#include "CRUDHandler.h"

#include <zmq.hpp>
#include <thread>
#include <atomic>

//-----------------------------------------------------------------------------------//

class MainComponent : public juce::AudioAppComponent, private juce::Timer, public IActionListener
{
public:
    //-----------------------------------------------------------------------------------//

    MainComponent();
    ~MainComponent() override;

    //-----------------------------------------------------------------------------------//

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //-----------------------------------------------------------------------------------//

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    //-----------------------------------------------------------------------------------//

    void timerCallback() override;
    void addSecondChain();

    //-----------------------------------------------------------------------------------//
    // Components

    Chain audioChain;
    juce::AudioProcessorGraph mixerGraph;
    juce::AudioProcessorGraph audioGraph;

    ChainController chainController;
    MixerController mixerController;

    std::unique_ptr<Dispatcher> dispatcher;
    std::unique_ptr<ActionHandler> actionHandler;
    std::unique_ptr<CRUDHandler> crudHandler;

    juce::AudioProcessorGraph::Node::Ptr simpleOscNode;
    juce::AudioProcessorGraph::Node::Ptr beeperNode;
    juce::AudioProcessorGraph::Node::Ptr delayNode;
    juce::AudioProcessorGraph::Node::Ptr simpleReverbNode;
    juce::AudioProcessorGraph::Node::Ptr gainNode;

    juce::AudioProcessorGraph::Node::Ptr outputNode;

    //-----------------------------------------------------------------------------------//
    // ZeroMQ

    zmq::context_t zmqContext;
    std::unique_ptr<zmq::socket_t> zmqSocket;

    void handleZeroMQMessage(const std::string& message);

    void sendZeroMQMessage(const std::string& message);

    //-----------------------------------------------------------------------------------//
    // Audio Management

    std::atomic<bool> isAudioRunning{ true };
    void toggleAudio();

    //-----------------------------------------------------------------------------------//

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
