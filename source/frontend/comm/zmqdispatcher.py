from PyQt5.QtCore import QThread, pyqtSignal
import zmq


class ZMQDispatcher(QThread):
    messageSent = pyqtSignal(str)
    messageReceived = pyqtSignal(str)

    def __init__(self, parent=None):
        super(ZMQDispatcher, self).__init__(parent)
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.socket.connect("tcp://localhost:5555")
        self.running = True
        self.messages = []

    def run(self):
        while self.running:
            if self.messages:
                message = self.messages.pop(0)
                self.socket.send_string(message)
                self.messageSent.emit(message)
                reply = self.socket.recv_string()
                self.messageReceived.emit(reply)

    def send_message(self, message):
        self.messages.append(message)

    def stop(self):
        self.running = False
        self.socket.close()
        self.context.term()
        self.wait()
