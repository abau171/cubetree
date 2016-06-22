import threading
import socket
import json

from .cube import Cube

class WorkerThread(threading.Thread):
    def __init__(self, socket):
        super().__init__()
        self.socket = socket
        self.read_file = socket.makefile("r")
        self.write_file = socket.makefile("w")
    def job_loop(self):
        cube = Cube()
        cube.shuffle(2)
        message = json.dumps(cube.get_state())
        self.write_file.write(message + "\n")
        self.write_file.flush()
    def run(self):
        print("new worker connected")
        self.job_loop()
        self.socket.close()
        print("worker connection closed")

class ServerThread(threading.Thread):
    def __init__(self, port):
        super().__init__()
        self.port = port
    def run(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
            print("new server")
            server_socket.bind((socket.gethostname(), self.port))
            server_socket.listen(5)
            while True:
                client_socket, address = server_socket.accept()
                worker = WorkerThread(client_socket)
                worker.start()
        print("server closed")

def start_server(port):
    #ServerThread(port).start()
    ServerThread(port).run()

