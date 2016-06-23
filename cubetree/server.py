import threading
import socket
import json

from .cube import Cube, decode_move_list
from .json_socket_proxy import JSONSocketProxy

class WorkerConnectionThread(threading.Thread):

    def __init__(self, connection):
        super().__init__()
        self.connection = connection

    def job_loop(self):
        while True:
            cube = Cube()
            cube.shuffle(13)
            print(cube)
            cur_depth = 0
            solution = None
            while solution is None:
                cur_depth += 1
                self.connection.write([cube.get_state(), cur_depth])
                possible_solution = self.connection.read()
                if possible_solution is not None:
                    solution = decode_move_list(possible_solution)
            print("SOLUTION:", solution)

    def run(self):
        print("new worker connected")
        self.job_loop()
        self.connection.close()
        print("worker connection closed")

class ServerThread(threading.Thread):

    def __init__(self, port):
        super().__init__()
        self.port = port

    def run(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
            print("new server")
            server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            server_socket.bind((socket.gethostname(), self.port))
            server_socket.listen(5)
            while True:
                client_socket, address = server_socket.accept()
                worker_connection = WorkerConnectionThread(JSONSocketProxy(client_socket))
                worker_connection.start()
        print("server closed")

def start_server(port):
    #ServerThread(port).start()
    ServerThread(port).run()

