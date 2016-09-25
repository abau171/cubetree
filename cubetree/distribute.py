import time
import asyncio
import multiprocessing
import highfive

from . import model
from . import lookup


JOB_DEPTH = 12
PROGRESS_BAR_SIZE = 60
PROGRESS_BAR_CHAR = "="


PROGRESS_BAR_INC = 1 / PROGRESS_BAR_SIZE


class CubeJob(highfive.Job):

    def __init__(self, cube, depth, partial_solution):

        self._cube = cube
        self._depth = depth
        self._partial_solution = partial_solution

    def get_call(self):

        return [self._cube.get_state(), self._depth]

    def get_result(self, response):

        if response is None:
            return None

        delta = model.Algorithm(response)
        solution = self._partial_solution + delta
        return solution


def gen_moves(last_face):

    for face_id in range(6):

        face = model.Face(face_id)

        if last_face is not None:
            pruned_opposite = face is last_face.opposite() and face_id < 3
            if face is last_face or pruned_opposite:
                continue

        for turn_type_id in range(1, 4):
            yield face, model.TurnType(turn_type_id)


def calculate_progress(face_id, turn_type_id, sub_progress):

    return (3 * face_id + (turn_type_id - 1) + sub_progress) / 18


def gen_jobs(cube, depth, partial_solution=model.Algorithm()):

    if depth <= JOB_DEPTH:
        yield CubeJob(cube, depth, partial_solution), 1
        return

    if len(partial_solution) == 0:
        last_face = None
    else:
        last_face = partial_solution[-1][0]

    for face, turn_type in gen_moves(last_face):

        child = cube.clone()
        child.turn(face, turn_type)

        delta = model.Algorithm([[face.value, turn_type.value]])
        child_partial = partial_solution + delta

        for job, sub_progress in gen_jobs(child, depth - 1, child_partial):
            progress = calculate_progress(
                    face.value, turn_type.value, sub_progress)
            yield job, progress


def progress_filter(jobs):

    cur_progress = 0

    for job, progress in jobs:

        while cur_progress < progress:
            print(PROGRESS_BAR_CHAR, end="", flush=True)
            cur_progress += PROGRESS_BAR_INC

        yield job

    while cur_progress < 1:
        print(PROGRESS_BAR_CHAR, end="", flush=True)
        cur_progress += PROGRESS_BAR_INC


async def solve(cube, master):

    if cube.is_solved():
        return model.Algorithm()

    for cur_depth in range(1, 21):

        print("DEPTH {:>2} [".format(cur_depth), end="", flush=True)

        job_generator = progress_filter(gen_jobs(cube, cur_depth))

        async with master.run(job_generator) as js:
            async for solution in js.results():
                if solution is not None:
                    print("")
                    return solution

        print("]")


async def solver_main(hostname, port):

    async with await highfive.start_master(
            host=hostname, port=port) as master:
        await command_loop(master)


async def command_loop(master):
    cur_cube = model.Cube()
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
            Providing a random seed makes the shuffle reproducible.
            Also prints the shuffle algorithm when finished.
'turn' : Performs a specified algorithm on the cube.
'solve' : Performs a distributed solve on the cube from its current state to the start state.
          The solution algorithm is printed when found.\
""")
        elif command == "show":
            print()
            print(cur_cube)
        elif command == "reset":
            cur_cube = model.Cube()
        elif command == "shuffle":
            shuffle_depth = -1
            while shuffle_depth < 0:
                try:
                    shuffle_depth = int(input("shuffle depth: "))
                except ValueError:
                    print("please enter a non-negative integer as the shuffle depth")
                if shuffle_depth < 0:
                    print("please enter a non-negative integer as the shuffle depth")
            seed = input("random seed (optional): ")
            if seed == "":
                shuffle_algorithm = cur_cube.shuffle(shuffle_depth)
            else:
                shuffle_algorithm = cur_cube.shuffle(shuffle_depth, seed)
            print("shuffle algorithm: {}".format(shuffle_algorithm))
        elif command == "turn":
            try:
                algorithm = model.Algorithm(input("algorithm: "))
                cur_cube.apply_algorithm(algorithm)
            except ValueError:
                print("invalid algorithm")
        elif command == "solve":
            start_time = time.time()
            solution = await solve(cur_cube, master)
            print("solution:", solution)
            time_elapsed = time.time() - start_time
            seconds_elapsed = int(time_elapsed % 60)
            minutes_elapsed = int(time_elapsed // 60)
            print("solve took {}m{}s".format(minutes_elapsed, seconds_elapsed))
        elif command == "":
            pass
        else:
            print("unknown command")


def run_solver(hostname, port):
    asyncio.get_event_loop().run_until_complete(solver_main(hostname, port))


def worker_search_depth(call):
    cube_state, depth = call
    cube = model.Cube(cube_state)
    solution = cube.search_depth(depth)
    if solution is None:
        print("-", end="", flush=True)
        return None
    else:
        print("X", end="", flush=True)
        return solution.get_state()


def run_worker_pool(hostname, port, num_workers):
    lookup.load_or_gen_lookups()
    if num_workers == 0:
        num_workers = multiprocessing.cpu_count()
    try:
        highfive.run_worker_pool(worker_search_depth, host=hostname,
                port=port, max_workers=num_workers)
    except KeyboardInterrupt:
        print("keyboard interrupt")

