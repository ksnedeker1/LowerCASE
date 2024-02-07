#include <JuceHeader.h>
#include "MainComponent.h"
#include "Audio.h"

//-----------------------------------------------------------------------------------//

MainComponent::MainComponent()
    : mixerController(mixerGraph),
    chainController(audioGraph),
    zmqContext(1),
    zmqSocket(std::make_unique<zmq::socket_t>(zmqContext, ZMQ_REP))
{
    setSize(800, 600);

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

    actionHandler = std::make_unique<ActionHandler>(this);
    crudHandler = std::make_unique<CRUDHandler>();
    dispatcher = std::make_unique<Dispatcher>(crudHandler.get(), actionHandler.get());

    zmqSocket->bind("tcp://*:5555");

    startTimer(100);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//-----------------------------------------------------------------------------------//

void MainComponent::timerCallback()
{
    zmq::pollitem_t items[] =
    {
        { static_cast<void*>(*zmqSocket), 0, ZMQ_POLLIN, 0 }
    };
    zmq::poll(&items[0], 1, 0);

    if (items[0].revents & ZMQ_POLLIN)
    {
        zmq::message_t message;
        zmqSocket->recv(message, zmq::recv_flags::none);
        handleZeroMQMessage(message.to_string());
    }
}

void MainComponent::addSecondChain() {
    chainController.updateChain(1, std::vector<int>{ 1, 2, 3, 4 });
}

//-----------------------------------------------------------------------------------//

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

//-----------------------------------------------------------------------------------//

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized() {}

//-----------------------------------------------------------------------------------//

void MainComponent::handleZeroMQMessage(const std::string& message)
{
    DBG("Received message: " + juce::String(message));
    dispatcher->dispatch(message, chainController);

    std::string graphDescription = chainController.getGraphDescription();
    sendZeroMQMessage(graphDescription);
}

void MainComponent::sendZeroMQMessage(const std::string& message)
{
    zmqSocket->send(zmq::buffer(message), zmq::send_flags::none);
}

//-----------------------------------------------------------------------------------//

void MainComponent::toggleAudio()
{
    if (isAudioRunning) {
        deviceManager.closeAudioDevice();
        isAudioRunning = false;
    }
    else {
        deviceManager.restartLastAudioDevice();
        isAudioRunning = true;
    }
}