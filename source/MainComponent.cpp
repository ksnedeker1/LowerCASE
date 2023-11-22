#include <JuceHeader.h>
#include "MainComponent.h"
#include "Audio.h"

//==============================================================================
MainComponent::MainComponent() 
    : mixerController(mixerGraph), chainController(audioGraph)
{
    setSize(800, 600);

    // Some platforms require permissions to open input channels
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted)
            {
                if (granted)
                {
                    setAudioChannels(0, 2);
                }
            });
    }
    else
    {
        setAudioChannels(0, 2);
    }
    //MixerController mixerController(mixerGraph);
    //mixerController.initialize();
    //
    //auto beeperProcessor = std::make_unique<Beeper>();
    //auto beeperNode = audioGraph.addNode(std::move(beeperProcessor));
    //if (beeperNode)
    //{
    //    mixerController.connectToChannel(beeperNode->nodeID, 0);
    //}

    //AudioGraphWrapper agw(mixerGraph);
    //agw.initialize();

    startTimer(3000);

    chainController.createNode(1, std::make_unique<Beeper>());

    chainController.createNode(2, std::make_unique<SimpleReverb>(0.4, 0.3, 1.0, 0.6, 0.0));
    
    chainController.createNode(3, std::make_unique<Gain>(0.2));

    chainController.createNode(4, std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(
        juce::AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::audioOutputNode));

    chainController.createChain(1, std::vector<int>{ 1, 3, 4 });

    //chainController.connectNodes(beeperExternalId, simpleReverbExternalId);
    //chainController.connectNodes(simpleReverbExternalId, gainExternalId);
    //chainController.connectNodes(gainExternalId, outputExternalId);
    //chainController.disconnectNodes(simpleReverbExternalId, gainExternalId);
    //chainController.connectNodes(simpleReverbExternalId, gainExternalId);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::timerCallback() {
    addSecondChain();
    stopTimer();
}

void MainComponent::addSecondChain() {
    chainController.updateChain(1, std::vector<int>{ 1, 2, 3, 4 });
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    audioGraph.setPlayConfigDetails(0, 2, sampleRate, samplesPerBlockExpected);
    audioGraph.prepareToPlay(sampleRate, samplesPerBlockExpected);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    juce::MidiBuffer midiMessages;
    audioGraph.processBlock(*bufferToFill.buffer, midiMessages);
}

void MainComponent::releaseResources()
{
    audioGraph.releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized() {}