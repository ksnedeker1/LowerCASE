import sys
from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5.QtGui import QTextCursor, QTextCharFormat, QColor
from gui.mainwindow import Ui_MainWindow
from gui.ptelistener import PTEListener
from gui.syntaxhighlighter import SyntaxHighlighter
from environment.scriptvalidator import ScriptValidator


class MainWindowController(QMainWindow, Ui_MainWindow):
    def __init__(self, module_json_file_path, *args, **kwargs):
        """
        :param module_json_file_path: Path to JSON file for dynamic loading of available modules
        """
        super(MainWindowController, self).__init__(*args, **kwargs)
        self.module_json_file_path = module_json_file_path
        # State values and flags
        self.audioRunning = True
        self.contains_errors = False
        self.recent_error_line = None
        self.last_error_line = None
        self.highlight_flag = False
        # Init methods
        self.setupUi(self)
        self.init_element_states()
        self.init_element_connections()
        self.init_pte_listener()
        self.init_script_validator()
        self.init_syntax_highlighter()

    def init_element_states(self):
        """
        Default states and styles for GUI elements
        """
        stylesheet = "./gui/resources/stylesheet.qss"
        with open(stylesheet, "r") as f:
            self.setStyleSheet(f.read())
        self.tempoSpinBox.setRange(0.0, 400.0)
        self.tempoSpinBox.setValue(120.0)
        self.masterVolumeSlider.setRange(0, 100)
        self.masterVolumeSlider.setValue(80)
        self.audioToggleButton.setText(" Stop Audio ")
        pte2stylesheet = "./gui/resources/pte2stylesheet.qss"
        with open(pte2stylesheet, "r") as f:
            self.logPlainTextEdit.setStyleSheet(f.read())
        self.logPlainTextEdit.setReadOnly(True)
        self.logPlainTextEdit.setWordWrapMode(True)
        self.logPlainTextEdit.setMaximumBlockCount(2000)

    def init_element_connections(self):
        """
        Default connections for signals from GUI elements
        """
        self.audioToggleButton.clicked.connect(self.on_click_audio_toggle)
        self.tempoSpinBox.valueChanged.connect(self.on_tempo_spin_changed)
        self.masterVolumeSlider.valueChanged.connect(self.on_master_volume_changed)
        self.revertScript.clicked.connect(self.handle_revert_script)
        self.highlightErrors.clicked.connect(self.highlight_pte_line)

    def init_pte_listener(self):
        self.pte_listener = PTEListener(self.scriptPlainTextEdit)
        self.pte_listener.textChanged.connect(self.validate_script)
        self.pte_listener.start()

    def init_script_validator(self):
        self.script_validator = ScriptValidator(self.module_json_file_path)

    def init_syntax_highlighter(self):
        self.syntax_highlighter = SyntaxHighlighter(self.scriptPlainTextEdit.document())

    def validate_script(self, script_text):
        """
        Validate script on textChanged signal
        """
        is_valid, error_line = self.script_validator.execute_and_validate_script(script_text)
        if not is_valid:
            self.contains_errors = True
            self.update_log("Script contains errors!")
            if error_line:
                self.recent_error_line = error_line
        else:
            self.contains_errors = False
            if self.highlight_flag:
                self.syntax_highlighter.clear_highlighting()
            self.update_log("Script is valid.")

    def handle_revert_script(self):
        """
        Revert to most recent valid script state if requested and current version is invalid
        """
        if self.scriptPlainTextEdit.toPlainText() == self.script_validator.last_valid_script:
            self.update_log("Current script version is valid!")
        else:
            self.scriptPlainTextEdit.setPlainText(self.script_validator.last_valid_script)

    def update_log(self, text):
        """
        Appends the provided text to the log display with terminal-like auto-scrolling
        """
        self.logPlainTextEdit.appendPlainText(text)
        # Auto-scroll to end
        cursor = self.logPlainTextEdit.textCursor()
        cursor.movePosition(QTextCursor.End)
        self.logPlainTextEdit.setTextCursor(cursor)

    def highlight_pte_line(self):
        """
        Highlight first script line with errors
        """
        if self.contains_errors and self.recent_error_line is not None:
            # Clear last error line if the previous error was resolved
            if self.recent_error_line != self.last_error_line:
                self.syntax_highlighter.clear_highlighting()
            self.highlight_flag = True
            self.last_error_line = self.recent_error_line
            self.syntax_highlighter.highlight_line(self.recent_error_line, '#550000')

    def on_click_audio_toggle(self):
        """
        Toggle audio generation
        """
        self.audioRunning = not self.audioRunning
        if self.audioRunning:
            self.audioToggleButton.setText(" Stop Audio ")
        else:
            self.audioToggleButton.setText(" Start Audio ")
        # TODO: Call exposed backend function to start/stop audio
        pass

    def on_tempo_spin_changed(self, value):
        """
        Handle tempo updates
        """
        # TODO: Call exposed backend function to update tempo
        pass

    def on_master_volume_changed(self, value):
        """
        Handle volume updates
        """
        volume = value / 100
        # TODO: Call exposed backend function to update master gain
        pass


def launch(module_json_file_path):
    app = QApplication(sys.argv)
    window = MainWindowController(module_json_file_path)
    window.show()
    sys.exit(app.exec_())
