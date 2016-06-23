import threading
import socket
import json

from .cube import Cube, Face, TurnType, decode_move_list
from .json_socket_proxy import JSONSocketProxy

def _encode_job(job):
    return [job[0].get_state(), job[1]]

class WorkerConnectionThread(threading.Thread):

    def __init__(self, connection, job_manager):
        super().__init__(daemon=True)
        self.connection = connection
        self.job_manager = job_manager

    def job_loop(self):
        while True:
            job = self.job_manager.get()
            self.connection.write(_encode_job(job))
            possible_solution = self.connection.read()
            if possible_solution is not None:
                solution = decode_move_list(possible_solution)
                self.job_manager.set_solution(solution)
            self.job_manager.job_done()

    def run(self):
        print("new worker connected")
        self.job_loop()
        self.connection.close()
        print("worker connection closed")

class WorkerListenerThread(threading.Thread):

    def __init__(self, port, job_manager):
        super().__init__(daemon=True)
        self.port = port
        self.job_manager = job_manager

    def run(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
            print("new server")
            server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            server_socket.bind((socket.gethostname(), self.port))
            server_socket.listen(5)
            while True:
                client_socket, address = server_socket.accept()
                worker_connection = WorkerConnectionThread(JSONSocketProxy(client_socket), self.job_manager)
                worker_connection.start()
        print("server closed")

class JobManager:

    def __init__(self):
        self.job_iterator = None
        self.next_job = None
        self.solution = None

        self.mutex = threading.Lock()
        self.not_empty = threading.Condition(self.mutex)
        self.all_jobs_done = threading.Condition(self.mutex)
        self.unfinished_jobs = 0

    def _try_get_next(self):
        # must have self.not_empty
        if self.job_iterator is None:
            return
        try:
            self.next_job = next(self.job_iterator)
            self.not_empty.notify()
        except StopIteration:
            self.next_job = None
            self.job_iterator = None

    def job_done(self):
        with self.all_jobs_done:
            if self.unfinished_jobs > 0:
                self.unfinished_jobs -= 1
                self.all_jobs_done.notify()
            else:
                raise ValueError("job_done() called too many times")

    def set_solution(self, solution):
        self.solution = solution

    def get_solution(self):
        return self.solution

    def join(self):
        with self.all_jobs_done:
            while self.next_job is not None or self.unfinished_jobs > 0:
                self.all_jobs_done.wait()

    def set_job_source(self, job_source):
        with self.not_empty:
            self.job_iterator = iter(job_source)
            self._try_get_next()

    def get(self):
        with self.not_empty:
            while self.next_job is None:
                self.not_empty.wait()
            job = self.next_job
            self.unfinished_jobs += 1
            self._try_get_next()
            return job

def gen_jobs(cube, depth):
    if depth > 13:
        for face_id in range(6):
            for turn_type_id in range(1, 4):
                clone_cube = Cube(cube.get_state())
                clone_cube.turn(Face(face_id), TurnType(turn_type_id))
                yield from gen_jobs(clone_cube, depth - 1)
    else:
        yield cube, depth

def start_server(port):
    job_manager = JobManager()
    WorkerListenerThread(port, job_manager).start()
    for i in range(5): # solve 5 random cubes
        cube = Cube()
        while cube.is_solved():
            cube.shuffle(14)
        print("SOLVING CUBE {}:".format(i + 1))
        print(cube)
        for cur_depth in range(1, 21):
            print("DEPTH", cur_depth)
            job_manager.set_solution(None)
            job_manager.set_job_source(gen_jobs(cube, cur_depth))
            job_manager.join()
            solution = job_manager.get_solution()
            if solution is not None:
                print("SOLUTION:", solution)
                break

