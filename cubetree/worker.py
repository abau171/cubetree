import socket
import threading
import json

from .cube import Cube, encode_move_list
from .json_socket_proxy import JSONSocketProxy

def _decode_job(job):
    return Cube(job[0]), job[1]

class WorkerThread(threading.Thread):

    def __init__(self, hostname, port):
        super().__init__()
        self.hostname = hostname
        self.port = port

    def job_loop(self):
        while True:
            job = self.connection.read()
            cube, depth = _decode_job(job)
            solution = cube.search_depth(depth)
            result = encode_move_list(solution) if solution is not None else None
            self.connection.write(result)

    def run(self):
        worker_socket = socket.socket()
        worker_socket.connect((self.hostname, self.port))
        self.connection = JSONSocketProxy(worker_socket)
        self.job_loop()
        self.connection.close()

def start_worker(hostname, port):
    WorkerThread(hostname, port).start()

