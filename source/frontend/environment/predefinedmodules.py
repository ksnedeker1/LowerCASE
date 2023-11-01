import uuid


class Node:
    def __init__(self):
        self.id = str(uuid.uuid4())
        self.connected_to = []
        self.connected_from = []


class Master(Node):
    def __init__(self):
        super().__init__()  # Node constructor assigns UUID

    def process(self, input_module=None):
        """Simulates signal flow, string for debugging"""
        if input_module:
            self.connected_from.append(input_module)
        return f"Final output to Master"


class Mixer(Node):
    def __init__(self, channel_number):
        super().__init__()  # Node constructor assigns UUID
        self.channel_number = channel_number

    def add_input(self, chain):
        if chain not in self.connected_from:
            self.connected_from.append(chain)
        if self not in chain.connected_to:
            chain.connected_to.append(self)

    def process(self):
        """Simulates signal flow, string for debugging"""
        combined_output = [input_module.process() for input_module in self.connected_from]
        return f"Mixer {self.channel_number} combined output: {combined_output}"


class Chain(Node):
    def __init__(self, modules):
        super().__init__()  # Node constructor assigns UUID
        self.modules = modules
        for idx, module in enumerate(self.modules[:-1]):
            if self.modules[idx + 1] not in module.connected_to:
                module.connected_to.append(self.modules[idx + 1])
            if module not in self.modules[idx + 1].connected_from:
                self.modules[idx + 1].connected_from.append(module)

    def process(self):
        """Simulates signal flow, string for debugging"""
        output = None
        for module in self.modules:
            output = module.process(output)
        return output


def chain(*args):
    return Chain(args)


def build_graph(objects):
    graph = {}
    for obj_name, obj in objects.items():
        if isinstance(obj, Node):
            graph[obj.id] = {
                "type": obj.__class__.__name__,
                "connected_to": [m.id for m in obj.connected_to],
                "connected_from": [m.id for m in obj.connected_from]
            }
            if isinstance(obj, Chain):
                graph[obj.id]["components"] = [component.id for component in obj.modules]
    return graph


Master = Master()
