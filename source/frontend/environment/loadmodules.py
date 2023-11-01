import json
from environment.predefinedmodules import Node
from environment.predefinedmodules import chain, Mixer, Master


def create_module_class(module_data):
    """
    Create a class based on the JSON definition of a module
    """
    class ModuleClass(Node):
        def __init__(self, *args, **kwargs):
            super().__init__()  # Node constructor assigns UUID
            # Map positional arguments to parameter names
            param_names = list(module_data["parameters"].keys())
            for index, arg in enumerate(args):
                if index < len(param_names):
                    setattr(self, param_names[index], arg)
            # For any remaining parameters not provided as positional args
            for param, details in module_data["parameters"].items():
                if param not in self.__dict__:
                    default_value = details["default"]
                    setattr(self, param, kwargs.get(param, default_value))

        def process(self, input_data=None):
            """Simulates signal flow, string for debugging"""
            return f"Processed by {self.__class__.__name__}"

    # Update class name
    ModuleClass.__name__ = module_data["name"]
    return ModuleClass


def load_modules(file_path):
    """
    Dynamically create class definitions for each module definition in the provided JSON
    """
    with open(file_path, 'r') as file:
        data = json.load(file)
    modules = {}
    for module_data in data["modules"]:
        class_name = module_data["name"]
        modules[class_name] = create_module_class(module_data)
    # chain(), Mixer, and Master are always present
    modules["chain"] = chain
    modules["Mixer"] = Mixer
    modules["Master"] = Master
    # inject namespace dict for scripting environment into global namespace
    globals().update(modules)
    return modules
