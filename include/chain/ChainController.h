#pragma once

#include <JuceHeader.h>
#include <unordered_map>
#include <set>
#include <vector>

/*------------------------------------------------------------------------------------\
| Manages an ordered chain of connected modules
\------------------------------------------------------------------------------------*/
class ChainController {
public:
    //-----------------------------------------------------------------------------------//

    ChainController(juce::AudioProcessorGraph& graph);

    //-----------------------------------------------------------------------------------//

    void clear();

    //-----------------------------------------------------------------------------------//
    // Node CRUD

    void createNode(int externalId, std::unique_ptr<juce::AudioProcessor> processor);
    void deleteNode(int externalId);

    //-----------------------------------------------------------------------------------//
    // Chain CRUD

    void createChain(int chainId, const std::vector<int>& nodeExternalIds);
    void updateChain(int chainId, const std::vector<int>& newExternalIds);
    void deleteChain(int chainId);

    //-----------------------------------------------------------------------------------//
    // AudioProcessorGraph Management

    void connectNodes(int sourceExternalId, int destExternalId);
    void disconnectNodes(int sourceExternalId, int destExternalId);

    //-----------------------------------------------------------------------------------//
    
    juce::AudioProcessorGraph::NodeID getNodeId(int externalId) const;
    const std::unordered_map<int, juce::AudioProcessorGraph::NodeID>& getNodeIdMap() const;
    const std::set<std::pair<int, int>>& getConnectionSet() const;
    const std::unordered_map<int, std::vector<int>>& getChainMap() const;

    std::string getGraphDescription() const;

private:
    //-----------------------------------------------------------------------------------//
    // Components

    juce::AudioProcessorGraph& audioGraph;

    std::unordered_map<int, juce::AudioProcessorGraph::NodeID> nodeIdMap;
    std::set<std::pair<int, int>> connectionSet;
    std::unordered_map<int, std::vector<int>> chainMap;
};