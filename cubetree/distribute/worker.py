import socket
import multiprocessing

import cubetree.lookup
import cubetree.distribute.json_socket_proxy


class WorkerProcess(multiprocessing.Process):

    def __init__(self, hostname, port):
        super().__init__()
        self.hostname = hostname
        self.port = port

    def job_loop(self):
        try:
            while True:
                job = self.connection.read()
                last_face = None if len(job.partial_solution.move_list) == 0 else job.partial_solution.move_list[-1][0]
                solution = job.cube.search_depth(job.depth, last_face)
                if solution is not None:
                    print("X", end="", flush=True)
                    self.connection.write(job.partial_solution + solution)
                else:
                    print(".", end="", flush=True)
                    self.connection.write(None)
        except KeyboardInterrupt:
            pass
        except cubetree.distribute.json_socket_proxy.EndOfStream:
            pass
        except OSError as e:
            pass

    def run(self):
        worker_socket = socket.socket()
        worker_socket.connect((self.hostname, self.port))
        self.connection = cubetree.distribute.json_socket_proxy.JSONSocketProxy(worker_socket)
        self.job_loop()
        self.connection.close()


_workers = set()


def start_worker(hostname, port):
    worker = WorkerProcess(hostname, port)
    worker.start()
    _workers.add(worker)


def join_workers():
    for worker in _workers:
        worker.join()


def terminate_workers():
    for worker in _workers:
        worker.terminate()


def run_worker(hostname, port, num_workers):
    cubetree.lookup.load_or_gen_lookups()
    if num_workers == 0:
        num_workers = multiprocessing.cpu_count()
    for i in range(num_workers):
        start_worker(hostname, port)
    try:
        join_workers()
    except KeyboardInterrupt:
        print("terminating workers...")
        terminate_workers()

