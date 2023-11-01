from environment.loadmodules import load_modules
from environment.predefinedmodules import *
import traceback
import re


class ScriptValidator:
    def __init__(self, module_json_file_path):
        self.modules = load_modules(module_json_file_path)
        self.last_valid_script = ""

    def execute_and_validate_script(self, script_str):
        """Tries to execute the script and returns info about validity and errors"""
        try:
            exec(script_str, self.modules)
            self.last_valid_script = script_str
            return True, None
        except Exception as e:
            # Extract the line number of the error
            tb = traceback.format_exc().split('\n')
            line_pattern = re.compile(r'File "<string>", line (\d+)')
            line_number = None
            for line in tb:
                match = line_pattern.search(line)
                if match:
                    line_number = int(match.group(1))
                    break
            # print(f"Error at line {line_number}: {e}")
            return False, line_number
