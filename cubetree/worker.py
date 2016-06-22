import socket
import threading
import json

from .cube import Cube

class WorkerThread(threading.Thread):
    def __init__(self, hostname, port):
        super().__init__()
        self.hostname = hostname
        self.port = port
    def job_loop(self):
        message = self.read_file.readline().rstrip("\n")
        cube = Cube(json.loads(message))
        print(cube)
    def run(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
            print("new worker")
            client_socket.connect((self.hostname, self.port))
            self.read_file = client_socket.makefile("r")
            self.write_file = client_socket.makefile("w")
            self.job_loop()
        print("worker closed")

def start_worker(hostname, port):
    WorkerThread(hostname, port).start()

