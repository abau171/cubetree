import random

from .cube import Cube
from .solver import search_depth, set_cancel_checker

# TEMPORARY FAKES

class FakeWorkerConnection:
    def __init__(self, hostname, port):
        pass
    def get_job(self):
        print("getting job")
        cube = Cube()
        cube.shuffle(100)
        return cube, 14
    def check_cancel(self):
        print("checking cancel")
        if random.random() > 0.9:
            print("CANCELLING")
            return True
        return False
    def send_result(self, result):
        print("RESULT: " + str(result))

# END FAKES

def worker(hostname, port):
    conn = FakeWorkerConnection(hostname, port)
    set_cancel_checker(conn.check_cancel)
    while True:
        cube, depth = conn.get_job()
        result = search_depth(cube, depth)
        conn.send_result(result)

