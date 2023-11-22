#include "ChainController.h"

ChainController::ChainController(juce::AudioProcessorGraph& graph)
    : audioGraph(graph) {}

void ChainController::clear() {
    audioGraph.clear();
    nodeIdMap.clear();
    connectionSet.clear();
    chainMap.clear();
}

void ChainController::createNode(int externalId, std::unique_ptr<juce::AudioProcessor> processor) {
    auto node = audioGraph.addNode(std::move(processor));
    if (node) {
        nodeIdMap[externalId] = node->nodeID;
    }
    else {
        DBG("Failed to add node to the audio graph");
    }
}

void ChainController::deleteNode(int externalId) {
    auto nodeId = getNodeId(externalId);
    audioGraph.removeNode(nodeId);
    nodeIdMap.erase(externalId);

    // Remove all connections involving this node
    for (auto it = connectionSet.begin(); it != connectionSet.end(); ) {
        if (it->first == externalId || it->second == externalId) {
            disconnectNodes(it->first, it->second);
            it = connectionSet.erase(it);
        }
        else {
            ++it;
        }
    }
}

juce::AudioProcessorGraph::NodeID ChainController::getNodeId(int externalId) const {
    auto it = nodeIdMap.find(externalId);
    if (it != nodeIdMap.end()) {
        return it->second;
    }
    else {
        DBG("Node ID not found for external ID: " << externalId);
        return juce::AudioProcessorGraph::NodeID();
    }
}

void ChainController::connectNodes(int sourceExternalId, int destExternalId) {
    auto sourceNodeId = getNodeId(sourceExternalId);
    auto destNodeId = getNodeId(destExternalId);

    for (int channel = 0; channel < 2; ++channel) {
        if (audioGraph.addConnection({ {sourceNodeId, channel}, {destNodeId, channel} })) {
            connectionSet.insert({ sourceExternalId, destExternalId });
        }
        else {
            DBG("Failed to connect nodes: " << sourceExternalId << " to " << destExternalId);
        }
    }
}

void ChainController::disconnectNodes(int sourceExternalId, int destExternalId) {
    auto sourceNodeId = getNodeId(sourceExternalId);
    auto destNodeId = getNodeId(destExternalId);

    auto connection = std::make_pair(sourceExternalId, destExternalId);
    if (connectionSet.find(connection) != connectionSet.end()) {
        for (int channel = 0; channel < 2; ++channel) {
            if (!audioGraph.removeConnection({ {sourceNodeId, channel}, {destNodeId, channel} })) {
                DBG("Failed to disconnect nodes: " << sourceExternalId << " to " << destExternalId);
            }
        }
        connectionSet.erase(connection);
    }
    else {
        DBG("Connection not found between nodes: " << sourceExternalId << " to " << destExternalId);
    }
}

void ChainController::createChain(int chainId, const std::vector<int>& nodeExternalIds) {
    if (nodeExternalIds.size() < 2) {
        DBG("Chain requires at least two nodes");
        return;
    }

    std::vector<int> chainNodes;

    for (size_t i = 0; i < nodeExternalIds.size() - 1; ++i) {
        connectNodes(nodeExternalIds[i], nodeExternalIds[i + 1]);
        chainNodes.push_back(nodeExternalIds[i]);
    }
    chainNodes.push_back(nodeExternalIds.back());
    chainMap[chainId] = chainNodes;
}

void ChainController::updateChain(int chainId, const std::vector<int>& newExternalIds) {
    auto chainIt = chainMap.find(chainId);
    if (chainIt != chainMap.end()) {
        // Disconnect existing chain nodes
        auto& existingNodes = chainIt->second;
        for (size_t i = 0; i < existingNodes.size() - 1; ++i) {
            disconnectNodes(existingNodes[i], existingNodes[i + 1]);
        }

        // Connect new chain nodes
        std::vector<int> newChainNodes;
        for (size_t i = 0; i < newExternalIds.size() - 1; ++i) {
            connectNodes(newExternalIds[i], newExternalIds[i + 1]);
            newChainNodes.push_back(newExternalIds[i]);
        }
        newChainNodes.push_back(newExternalIds.back());

        // Update chain map
        chainMap[chainId] = newChainNodes;
    }
    else {
        DBG("Chain with ID " << chainId << " does not exist");
    }
}

void ChainController::deleteChain(int chainId) {
    auto chainIt = chainMap.find(chainId);
    if (chainIt != chainMap.end()) {
        const auto& chainNodes = chainIt->second;
        for (size_t i = 0; i < chainNodes.size() - 1; ++i) {
            disconnectNodes(chainNodes[i], chainNodes[i + 1]);
        }
        chainMap.erase(chainId);
    }
    else {
        DBG("Chain with ID " << chainId << " does not exist");
    }
}

const std::unordered_map<int, juce::AudioProcessorGraph::NodeID>& ChainController::getNodeIdMap() const {
    return nodeIdMap;
}

const std::set<std::pair<int, int>>& ChainController::getConnectionSet() const {
    return connectionSet;
}

const std::unordered_map<int, std::vector<int>>& ChainController::getChainMap() const {
    return chainMap;
}
