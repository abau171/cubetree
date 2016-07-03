import socket
import threading
import time

import cubetree.cube
import cubetree.distribute.json_socket_proxy
import cubetree.distribute.job_manager


class WorkerConnectionThread(threading.Thread):

    def __init__(self, connection, job_manager):
        super().__init__(daemon=True)
        self.connection = connection
        self.job_manager = job_manager

    def job_loop(self):
        while True:
            job = self.job_manager.get()
            try:
                self.connection.write(job)
                solution = self.connection.read()
            except cubetree.distribute.json_socket_proxy.EndOfStream:
                self.job_manager.return_job(job)
                return
            except OSError as e:
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
                worker_connection = WorkerConnectionThread(cubetree.distribute.json_socket_proxy.JSONSocketProxy(client_socket), self.job_manager)
                worker_connection.start()


def gen_jobs(cube, depth, partial_solution=cubetree.cube.Algorithm()):
    if depth > 14:
        for face_id in range(6):
            face = cubetree.cube.Face(face_id)
            last_face = None if len(partial_solution.move_list) == 0 else partial_solution.move_list[-1][0]
            if last_face is not None:
                if face is last_face or (face.value < 3 and face is last_face.opposite()):
                    continue
            for turn_type_id in range(1, 4):
                turn_type = cubetree.cube.TurnType(turn_type_id)
                clone_cube = cubetree.cube.Cube(cube.get_state())
                clone_cube.turn(face, turn_type)
                yield from gen_jobs(clone_cube, depth - 1, partial_solution + cubetree.cube.Algorithm([(cubetree.cube.Face(face_id), cubetree.cube.TurnType(turn_type_id))]))
    else:
        yield cubetree.distribute.job_manager.Job(cube, depth, partial_solution)


class DistributedSolver:

    def __init__(self, hostname, port):
        self.job_manager = cubetree.distribute.job_manager.JobManager()
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

def run_solver(hostname, port):
    solver = DistributedSolver(hostname, port)
    cur_cube = cubetree.cube.Cube()
    print("type 'commands' to view a list of commands\n")
    while True:
        command = input("> ")
        if command == "exit":
            break
        elif command == "commands":
            print("""\
'exit' : Exits the program immediately.
'commands' : Shows the command list.
'show' : Displays the cube in its current state.
'reset' : Resets the cube to a new, solved cube.
'shuffle' : Shuffles the cube a specified number of times from its current state.
            Also prints the shuffle algorithm when finished.
'turn' : Performs a specified algorithm on the cube.
'solve' : Performs a distributed solve on the cube from its current state to the start state.
          The solution algorithm is printed when found.\
""")
        elif command == "show":
            print()
            print(cur_cube)
        elif command == "reset":
            cur_cube = cubetree.cube.Cube()
        elif command == "shuffle":
            shuffle_depth = -1
            while shuffle_depth < 0:
                try:
                    shuffle_depth = int(input("shuffle depth: "))
                except ValueError:
                    print("please enter a non-negative integer as the shuffle depth")
                if shuffle_depth < 0:
                    print("please enter a non-negative integer as the shuffle depth")
            shuffle_algorithm = cur_cube.shuffle(shuffle_depth)
            print("shuffle algorithm: {}".format(shuffle_algorithm))
        elif command == "turn":
            try:
                algorithm = cubetree.cube.Algorithm(input("algorithm: "))
                cur_cube.apply_algorithm(algorithm)
            except ValueError:
                print("invalid algorithm")
        elif command == "solve":
            start_time = time.time()
            solution = solver.solve(cur_cube)
            print("solution:", solution)
            time_elapsed = time.time() - start_time
            seconds_elapsed = int(time_elapsed % 60)
            minutes_elapsed = int(time_elapsed // 60)
            print("solve took {}m{}s".format(minutes_elapsed, seconds_elapsed))
        elif command == "":
            pass
        else:
            print("unknown command")

