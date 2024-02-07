import subprocess
import atexit
from gui.mainwindowcontroller import launch
from environment.local import PATH_TO_EXE


def stop_backend(process):
    process.terminate()


backend = subprocess.Popen([PATH_TO_EXE])
atexit.register(stop_backend, backend)

launch('')
