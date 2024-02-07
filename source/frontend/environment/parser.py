from comm.msgreg import *
import re


class ScriptParser:
    def __init__(self, script):
        self.script = script
        self.parsed_data = []
        self.name_to_id = {}
        self.chain_to_elements = {}
        self.current_id = 0
        self.parse_script()

    def generate_id(self):
        self.current_id += 1
        return self.current_id

    def names_to_ids(self, names):
        return [self.name_to_id[name.strip()] for name in names.split(',') if name.strip() in self.name_to_id]

    def parse_script(self):
        if self.script is not None:
            lines = self.script.split('\n')
            for line in lines:
                if line.strip():
                    # Pattern: word, any whitespace, '=', any whitespace, word, '(', any characters, ')'
                    # Represents: node_name = module_name(parameters)
                    match = re.match(r'(\w+)\s*=\s*(\w+)\((.*)\)', line)
                    if match:
                        node_name, module_name, parameters = match.groups()
                        if node_name not in self.name_to_id:
                            self.name_to_id[node_name] = self.generate_id()
                        node_id = self.name_to_id[node_name]
                        if module_name == 'Chain':
                            element_ids = self.names_to_ids(parameters)
                            self.chain_to_elements[node_id] = element_ids
                        self.parsed_data.append((node_id, node_name, module_name, parameters))

    def get_parsed_data(self):
        return self.parsed_data

    def get_name_to_id_map(self):
        return self.name_to_id

    def get_chain_to_elements_map(self):
        return self.chain_to_elements

    def to_command(self, node_id, node_name, module_name, parameters):
        command_prefix = CRUD_HANDLER
        operation = CR_OP

        if module_name == "Chain":
            element_ids = ','.join(map(str, self.chain_to_elements[node_id]))
            return f"{command_prefix}{operation}{CHAIN}{node_id}|{element_ids}"
        else:
            # Map module names to their string constants
            module_constants = {
                "SimpleOsc": SIMPLE_OSC,
                "Beeper": BEEPER,
                "SimpleReverb": SIMPLE_REVERB,
                "Delay": DELAY,
                "Gain": GAIN,
                "Out": OUT
            }
            module_constant = module_constants.get(module_name, INVALID_MODULE)
            if module_constant == INVALID_MODULE:
                return INVALID_MODULE
            formatted_params = parameters.replace(' ', '').replace(',', ',')
            return f"{command_prefix}{operation}{module_constant}{node_id}|{formatted_params}"

    def generate_commands(self):
        commands = []
        for node_id, node_name, module_name, parameters in self.parsed_data:
            command = self.to_command(node_id, node_name, module_name, parameters)
            commands.append(command)
        return commands
