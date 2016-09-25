import multiprocessing
import highfive

from . import model
from . import lookup


JOB_DEPTH = 14
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

