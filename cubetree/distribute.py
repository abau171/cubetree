import collections
import threading
import multiprocessing
import socket
import json
import sys

import cubetree.cube
import cubetree.json_socket_proxy


class Job(cubetree.json_socket_proxy.JSONSerializable):

    def __init__(self, cube, depth, partial_solution):
        self.cube = cube
        self.depth = depth
        self.partial_solution = partial_solution

    @classmethod
    def json_serialize(cls, obj):
        return [obj.cube.get_state(), obj.depth, [[face.value, turn_type.value] for face, turn_type in obj.partial_solution]]

    @classmethod
    def json_deserialize(cls, obj):
        return Job(cubetree.cube.Cube(obj[0]), obj[1], cubetree.cube.Algorithm((cubetree.cube.Face(move[0]), cubetree.cube.TurnType(move[1])) for move in obj[2]))


class WorkerConnectionThread(threading.Thread):

    def __init__(self, connection, job_manager):
        super().__init__(daemon=True)
        self.connection = connection
        self.job_manager = job_manager

    def job_loop(self):
        while True:
            job = self.job_manager.get()
            self.connection.write(job)
            try:
                solution = self.connection.read()
            except cubetree.json_socket_proxy.EndOfStream:
                self.job_manager.return_job(job)
                return
            if solution is not None:
                self.job_manager.set_solution(solution)
            self.job_manager.job_done()

    def run(self):
        self.job_loop()
        self.connection.close()


class WorkerListenerThread(threading.Thread):

    def __init__(self, hostname, port, job_manager):
        super().__init__(daemon=True)
        self.hostname = hostname
        self.port = port
        self.job_manager = job_manager

    def run(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
            server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            server_socket.bind((self.hostname, self.port))
            server_socket.listen(5)
            while True:
                client_socket, address = server_socket.accept()
                worker_connection = WorkerConnectionThread(cubetree.json_socket_proxy.JSONSocketProxy(client_socket), self.job_manager)
                worker_connection.start()


class JobManager:

    def __init__(self):
        self.job_iterator = None
        self.returned_jobs = collections.deque()
        self.next_job = None
        self.solution = None

        self.mutex = threading.Lock()
        self.not_empty = threading.Condition(self.mutex)
        self.all_jobs_done = threading.Condition(self.mutex)
        self.unfinished_jobs = 0

    def _try_get_next(self):
        # must have self.not_empty
        if len(self.returned_jobs) > 0:
            self.next_job = self.returned_jobs.popleft()
            return
        if self.job_iterator is None:
            self.next_job = None
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

    def return_job(self, job):
        with self.not_empty:
            if self.next_job is None:
                self.next_job = job
                self.not_empty.notify()
            else:
                self.returned_jobs.append(job)
            self.unfinished_jobs -= 1

    def set_solution(self, solution):
        self.solution = solution

    def get_solution(self):
        return self.solution

    def join(self):
        with self.all_jobs_done:
            while self.next_job is not None or len(self.returned_jobs) > 0 or self.unfinished_jobs > 0:
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


def gen_jobs(cube, depth, partial_solution=cubetree.cube.Algorithm()):
    if depth > 14:
        for face_id in range(6):
            for turn_type_id in range(1, 4):
                clone_cube = cubetree.cube.Cube(cube.get_state())
                clone_cube.turn(cubetree.cube.Face(face_id), cubetree.cube.TurnType(turn_type_id))
                yield from gen_jobs(clone_cube, depth - 1, partial_solution + cubetree.cube.Algorithm([(cubetree.cube.Face(face_id), cubetree.cube.TurnType(turn_type_id))]))
    else:
        yield Job(cube, depth, partial_solution)


class DistributedSolver:

    def __init__(self, hostname, port):
        self.job_manager = JobManager()
        WorkerListenerThread(hostname, port, self.job_manager).start()

    def solve(self, cube):
        if cube.is_solved():
            return cubetree.cube.Algorithm()
        for cur_depth in range(1, 21):
            print("DEPTH", cur_depth)
            self.job_manager.set_solution(None)
            self.job_manager.set_job_source(gen_jobs(cube, cur_depth))
            self.job_manager.join()
            solution = self.job_manager.get_solution()
            if solution is not None:
                return solution


class WorkerProcess(multiprocessing.Process):

    def __init__(self, hostname, port):
        super().__init__()
        self.hostname = hostname
        self.port = port

    def job_loop(self):
        try:
            while True:
                job = self.connection.read()
                solution = job.cube.search_depth(job.depth)
                if solution is not None:
                    print("X", end="")
                    sys.stdout.flush()
                    self.connection.write(job.partial_solution + solution)
                else:
                    print(".", end="")
                    sys.stdout.flush()
                    self.connection.write(None)
        except cubetree.json_socket_proxy.EndOfStream:
            pass

    def run(self):
        worker_socket = socket.socket()
        worker_socket.connect((self.hostname, self.port))
        self.connection = cubetree.json_socket_proxy.JSONSocketProxy(worker_socket)
        self.job_loop()
        self.connection.close()

def start_worker(hostname, port):
    WorkerProcess(hostname, port).start()

