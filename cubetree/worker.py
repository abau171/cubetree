import socket
import multiprocessing
import json

from .cube import Cube
from .json_socket_proxy import JSONSocketProxy

def _decode_job(job):
    return Cube(job[0]), job[1]

class WorkerProcess(multiprocessing.Process):

    def __init__(self, hostname, port):
        super().__init__()
        self.hostname = hostname
        self.port = port

    def job_loop(self):
        while True:
            job = self.connection.read()
            cube, depth = _decode_job(job)
            print("DEPTH", depth)
            solution = cube.search_depth(depth)
            self.connection.write(solution)

    def run(self):
        worker_socket = socket.socket()
        worker_socket.connect((self.hostname, self.port))
        self.connection = JSONSocketProxy(worker_socket)
        self.job_loop()
        self.connection.close()

def start_worker(hostname, port):
    WorkerProcess(hostname, port).start()

