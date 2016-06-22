import socket
import threading
import json

from .cube import Cube, encode_move_list

class WorkerThread(threading.Thread):
    def __init__(self, hostname, port):
        super().__init__()
        self.hostname = hostname
        self.port = port
    def job_loop(self):
        while True:
            message = self.read_file.readline().rstrip("\n")
            job = json.loads(message)
            cube = Cube(job[0])
            depth = job[1]
            print("SOLVING DEPTH {}", depth)
            l = cube.search_depth(depth)
            result = encode_move_list(l) if l is not None else None
            result_message = json.dumps(result)
            self.write_file.write(result_message + "\n")
            self.write_file.flush()
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

