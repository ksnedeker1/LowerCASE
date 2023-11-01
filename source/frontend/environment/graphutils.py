import networkx as nx
import matplotlib.pyplot as plt
from environment.predefinedmodules import Node, Chain


class InternalNode:
    def __init__(self, node_id, node_type):
        self.id = node_id
        self.type = node_type
        self.connected_to = []
        self.connected_from = []
        self.components = []  # Chain nodes only
        self.active = False


class GraphDiagnostics:
    def __init__(self):
        self.internal_graph = {}

    def build_graph(self, objects):
        """
        Build an internal representation of the graph based on the global objects created by the user's script
        """
        temp_graph = {}
        # Create node instances
        for obj_name, obj in objects.items():
            if isinstance(obj, Node):
                node_data = {
                    "type": obj.__class__.__name__,
                    "connected_to": [m.id for m in obj.connected_to],
                    "connected_from": [m.id for m in obj.connected_from],
                    "components": []
                }
                if isinstance(obj, Chain):
                    node_data["components"] = [component.id for component in obj.modules]
                temp_graph[obj.id] = node_data
                self.internal_graph[obj.id] = InternalNode(obj.id, node_data["type"])
        # Link nodes
        for node_id, data in temp_graph.items():
            internal_node = self.internal_graph[node_id]
            for connected_id in data["connected_to"]:
                internal_node.connected_to.append(self.internal_graph[connected_id])
            for connected_from_id in data["connected_from"]:
                internal_node.connected_from.append(self.internal_graph[connected_from_id])
            if data["components"]:
                for component_id in data["components"]:
                    internal_node.components.append(self.internal_graph[component_id])

        # Recursive helper to determine if a node is active
        # Works back from Mixer and Master nodes, marking all children as active
        def mark_active(node):
            if not node.active:
                node.active = True
                for connection in node.connected_from:
                    mark_active(connection)
                # If the node is a chain, also mark its components as active
                if node.type == "Chain":
                    for component in node.components:
                        mark_active(component)

        for node in self.internal_graph.values():
            if node.type in ["Master", "Mixer"]:
                mark_active(node)
        return self.internal_graph

    def log_graph(self):
        """
        Converts internal graph representation to human-readable text
        """
        log = ""
        module_type_counts = {}
        module_id_to_count = {}
        for node in self.internal_graph.values():
            module_type = node.type
            module_type_counts[module_type] = module_type_counts.get(module_type, 0) + 1
            module_id_to_count[node.id] = module_type_counts[module_type]
        # Display the connections
        for node in self.internal_graph.values():
            module_name = node.type
            module_count = module_id_to_count[node.id]
            sent_connections = [f"{connected_node.type} ({module_id_to_count[connected_node.id]})"
                                for connected_node in node.connected_to]
            received_connections = [f"{connected_node.type} ({module_id_to_count[connected_node.id]})"
                                    for connected_node in node.connected_from]
            status = "active" if node.active else "inactive"
            if sent_connections:
                sent_str = ", ".join(sent_connections)
                log += f"{module_name} ({module_count}) is {status} and connects to: {sent_str}\n"
            if received_connections:
                received_str = ", ".join(received_connections)
                log += f"{module_name} ({module_count}) is {status} and receives from: {received_str}\n"
            if not sent_connections and not received_connections:
                log += f"{module_name} ({module_count}) is {status} and has no connections.\n"
        return log

    def visualize_graph(self):
        """
        Create networkX directed graph visual of internal graph representation
        """
        G = nx.DiGraph()
        # nodes
        for node in self.internal_graph.values():
            G.add_node(node.id, type=node.type, active=node.active)
        # edges
        for node in self.internal_graph.values():
            for connected_node in node.connected_to:
                G.add_edge(node.id, connected_node.id)
        # Separate nodes by levels
        levels = {}
        for node_id in G:
            node_type = G.nodes[node_id]["type"]
            if node_type not in levels:
                levels[node_type] = []
            levels[node_type].append(node_id)
        # Define positions based on levels
        pos = {}
        level_order = ["Master", "Mixer", "Chain", "SimpleOsc", "Reverb"]
        for index, level in enumerate(level_order):
            nodes = levels.get(level, [])
            for idx, node in enumerate(nodes):
                pos[node] = ((idx + 1) * (10 / (len(nodes) + 1)), len(level_order) - index)
        # Node labels
        labels = {}
        for node in G.nodes:
            node_type = G.nodes[node]["type"]
            same_type_nodes = [n for n in G.nodes if G.nodes[n]["type"] == node_type]
            index = same_type_nodes.index(node) + 1
            labels[node] = f"{node_type} ({index})"
        # Color based on active / inactive
        color_map = ["lime" if G.nodes[node]["active"] else "red" for node in G.nodes]
        # Draw
        plt.figure(figsize=(10, 10))
        nx.draw(G, pos, labels=labels, with_labels=True, node_size=300, node_color=color_map, node_shape="o",
                font_size=10, font_weight='bold')
        plt.title("Chains Graph Visualization")
        plt.show()
