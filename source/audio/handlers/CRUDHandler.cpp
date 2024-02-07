#include "CRUDHandler.h"
#include "Audio.h"
#include <string>
#include <sstream>
#include <vector>
#include <JuceHeader.h>

//-----------------------------------------------------------------------------------//
// Maps ModuleCRUDHandlers to their associated moduleName string

HandlerFactory::HandlerFactory()
{
    moduleHandlers["Beeper"] = std::make_unique<BeeperHandler>();
    moduleHandlers["SimpleOsc"] = std::make_unique<SimpleOscHandler>();
    moduleHandlers["SimpleReverb"] = std::make_unique<SimpleReverbHandler>();
    moduleHandlers["Delay"] = std::make_unique<DelayHandler>();
    moduleHandlers["Gain"] = std::make_unique<GainHandler>();
    moduleHandlers["Out"] = std::make_unique<OutHandler>();
    moduleHandlers["Chain"] = std::make_unique<ChainHandler>();
}

IModuleCRUDHandler* HandlerFactory::getHandler(const std::string& moduleName)
{
    auto it = moduleHandlers.find(moduleName);
    if (it != moduleHandlers.end()) { return it->second.get(); }
    return nullptr;
}


//-----------------------------------------------------------------------------------//
// Parses a CRUD message and calls the appropriate method of the matching ModuleCRUDHandler.

void CRUDHandler::handle(const std::string& message, ChainController& chainController) 
{
    if (message == "CLEAR_ALL")
    {
        chainController.clear();
        return;
    }

    // Parse operation type, module name, node ID
    std::istringstream messageStream(message);
    std::string operation, moduleName, nodeIdStr, paramsStr;
    std::vector<std::string> params;

    std::getline(messageStream, operation, ':');
    std::getline(messageStream, moduleName, ':');
    std::getline(messageStream, nodeIdStr, '|');
    int nodeId = std::stoi(nodeIdStr);

    // Extract parameters if present
    size_t pos = message.find('|');
    if (pos != std::string::npos && pos + 1 < message.length())
    {
        paramsStr = message.substr(pos + 1);
        std::istringstream paramStream(paramsStr);
        std::string param;
        while (std::getline(paramStream, param, ','))
        {
            params.push_back(param);
        }
    }

    HandlerFactory hf;
    auto handler = hf.getHandler(moduleName);
    if (handler != nullptr)
    {
        if (operation == "C")
        {
            handler->create(nodeId, params, chainController);
        }
        else if (operation == "R")
        {
            handler->read(nodeId, chainController);
        }
        else if (operation == "U")
        {
            handler->update(nodeId, params, chainController);
        }
        else if (operation == "D")
        {
            handler->del(nodeId, chainController);
        }
    }
}

//-----------------------------------------------------------------------------------//
// Beeper

void BeeperHandler::create(int nodeId, const std::vector<std::string>& params, ChainController& chainController) {
    std::optional<float> frequency;
    std::optional<float> soundDurationProportion;

    // Parse parameters if provided
    if (params.size() >= 1) { frequency = std::stof(params[0]); }
    if (params.size() >= 2) { soundDurationProportion = std::stof(params[1]); }

    if (frequency.has_value() && soundDurationProportion.has_value())
    {
        chainController.createNode(nodeId, std::make_unique<Beeper>(frequency.value(), soundDurationProportion.value()));
    }
    else if (frequency.has_value())
    {
        chainController.createNode(nodeId, std::make_unique<Beeper>(frequency.value()));
    }
    else
    {
        chainController.createNode(nodeId, std::make_unique<Beeper>());
    }
}

void BeeperHandler::read(int nodeId, ChainController& chainController)
{}

void BeeperHandler::update(int nodeId, const std::vector<std::string>& params, ChainController& chainController)
{}

void BeeperHandler::del(int nodeId, ChainController& chainController)
{
    chainController.deleteNode(nodeId);
}

//-----------------------------------------------------------------------------------//
// SimpleOsc

void SimpleOscHandler::create(int nodeId, const std::vector<std::string>& params, ChainController& chainController)
{
    chainController.createNode(nodeId, std::make_unique<SimpleOsc>());
}

void SimpleOscHandler::read(int nodeId, ChainController& chainController)
{}

void SimpleOscHandler::update(int nodeId, const std::vector<std::string>& params, ChainController& chainController)
{}

void SimpleOscHandler::del(int nodeId, ChainController& chainController)
{
    chainController.deleteNode(nodeId);
}

//-----------------------------------------------------------------------------------//
// SimpleReverb

void SimpleReverbHandler::create(int nodeId, const std::vector<std::string>& params, ChainController& chainController)
{
    chainController.createNode(nodeId, std::make_unique<SimpleReverb>());
}

void SimpleReverbHandler::read(int nodeId, ChainController& chainController)
{}

void SimpleReverbHandler::update(int nodeId, const std::vector<std::string>& params, ChainController& chainController)
{}

void SimpleReverbHandler::del(int nodeId, ChainController& chainController)
{
    chainController.deleteNode(nodeId);
}

//-----------------------------------------------------------------------------------//
// DelayHandler

void DelayHandler::create(int nodeId, const std::vector<std::string>& params, ChainController& chainController)
{
    chainController.createNode(nodeId, std::make_unique<Delay>());
}

void DelayHandler::read(int nodeId, ChainController& chainController)
{}

void DelayHandler::update(int nodeId, const std::vector<std::string>& params, ChainController& chainController)
{}

void DelayHandler::del(int nodeId, ChainController& chainController)
{
    chainController.deleteNode(nodeId);
}

//-----------------------------------------------------------------------------------//
// Gain

void GainHandler::create(int nodeId, const std::vector<std::string>& params, ChainController& chainController)
{
    std::optional<float> initialGain;

    if (!params.empty())
    {
        initialGain = std::stof(params.front());
        chainController.createNode(nodeId, std::make_unique<Gain>(initialGain.value()));
    }
    else
    {
        chainController.createNode(nodeId, std::make_unique<Gain>());
    }
}

void GainHandler::read(int nodeId, ChainController& chainController)
{}

void GainHandler::update(int nodeId, const std::vector<std::string>& params, ChainController& chainController)
{}

void GainHandler::del(int nodeId, ChainController& chainController)
{
    chainController.deleteNode(nodeId);
}

//-----------------------------------------------------------------------------------//
// Out

void OutHandler::create(int nodeId, const std::vector<std::string>& params, ChainController& chainController) {
    chainController.createNode(nodeId, std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(
        juce::AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::audioOutputNode));
}

void OutHandler::read(int nodeId, ChainController& chainController)
{}

void OutHandler::update(int nodeId, const std::vector<std::string>& params, ChainController& chainController)
{}

void OutHandler::del(int nodeId, ChainController& chainController)
{
    chainController.deleteNode(nodeId);
}

//-----------------------------------------------------------------------------------//
// Chain

void ChainHandler::create(int chainId, const std::vector<std::string>& params, ChainController& chainController)
{
    std::vector<int> nodeIds;

    for (const auto& param : params) {
        nodeIds.push_back(std::stoi(param));
    }

    chainController.createChain(chainId, nodeIds);
}

void ChainHandler::read(int nodeId, ChainController& chainController)
{}

void ChainHandler::update(int chainId, const std::vector<std::string>& params, ChainController& chainController)
{
    std::vector<int> nodeIds;

    for (const auto& param : params) {
        nodeIds.push_back(std::stoi(param));
    }

    chainController.updateChain(chainId, nodeIds);
}

void ChainHandler::del(int chainId, ChainController& chainController)
{
    chainController.deleteChain(chainId);
}