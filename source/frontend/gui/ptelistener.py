from PyQt5.QtCore import QThread, pyqtSignal


class PTEListener(QThread):
    textChanged = pyqtSignal(str)

    def __init__(self, edit):
        super().__init__()
        self.edit = edit
        self.previous_text = ""

    def run(self):
        """Continuously checks the QPlainTextEdit for updates with polling rate 100ms"""
        while True:
            current_text = self.edit.toPlainText()
            if current_text != self.previous_text:
                self.textChanged.emit(current_text)
                self.previous_text = current_text
            QThread.msleep(100)
