import collections
import threading

import cubetree.cube
import cubetree.distribute.json_socket_proxy


class Job(cubetree.distribute.json_socket_proxy.JSONSerializable):

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
        # must have mutex
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
            self.unfinished_jobs -= 1
            self.all_jobs_done.notify()
            if self.solution is None:
                if self.next_job is None:
                    self.next_job = job
                    self.not_empty.notify()
                else:
                    self.returned_jobs.append(job)

    def set_solution(self, solution):
        with self.mutex:
            self.solution = solution
            self.job_iterator = None
            self.returned_jobs.clear()
            self.next_job = None

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

