#pragma once

#include <JuceHeader.h>
#include <unordered_map>
#include <set>
#include <vector>

class ChainController {
public:
    ChainController(juce::AudioProcessorGraph& graph);
    void clear();

    // Node ops
    void createNode(int externalId, std::unique_ptr<juce::AudioProcessor> processor);
    void deleteNode(int externalId);
    juce::AudioProcessorGraph::NodeID getNodeId(int externalId) const;

    // Connection ops
    void connectNodes(int sourceExternalId, int destExternalId);
    void disconnectNodes(int sourceExternalId, int destExternalId);

    // Chain ops
    void createChain(int chainId, const std::vector<int>& nodeExternalIds);
    void updateChain(int chainId, const std::vector<int>& newExternalIds);
    void deleteChain(int chainId);

    // Getters
    const std::unordered_map<int, juce::AudioProcessorGraph::NodeID>& getNodeIdMap() const;
    const std::set<std::pair<int, int>>& getConnectionSet() const;
    const std::unordered_map<int, std::vector<int>>& getChainMap() const;

private:
    juce::AudioProcessorGraph& audioGraph;
    std::unordered_map<int, juce::AudioProcessorGraph::NodeID> nodeIdMap;
    std::set<std::pair<int, int>> connectionSet;
    std::unordered_map<int, std::vector<int>> chainMap;
};