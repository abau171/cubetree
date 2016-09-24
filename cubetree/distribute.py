import time
import asyncio
import multiprocessing
import highfive

import cubetree.cube


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

        delta = cubetree.cube.Algorithm(
                (cubetree.cube.Face(move[0]), cubetree.cube.TurnType(move[1]))
                for move in response)
        solution = self._partial_solution + delta
        return solution


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
        yield CubeJob(cube, depth, partial_solution)


async def solve(cube, master):

    if cube.is_solved():
        return cubetree.cube.Algorithm()
    for cur_depth in range(1, 21):

        print("DEPTH", cur_depth)

        async with master.run(gen_jobs(cube, cur_depth)) as js:
            async for solution in js.results():
                if solution is not None:
                    return solution


async def solver_main(hostname, port):
    master = await highfive.start_master(host=hostname, port=port)
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
    cube = cubetree.cube.Cube(cube_state)
    solution = cube.search_depth(depth)
    if solution is None:
        print("-", end="", flush=True)
        return None
    else:
        print("X", end="", flush=True)
        solution_state = [[face.value, turn_type.value]
                for face, turn_type in solution]
        return solution_state


def run_worker_pool(hostname, port, num_workers):
    cubetree.lookup.load_or_gen_lookups()
    if num_workers == 0:
        num_workers = multiprocessing.cpu_count()
    try:
        highfive.run_worker_pool(worker_search_depth, host=hostname,
                port=port, max_workers=num_workers)
    except KeyboardInterrupt:
        print("keyboard interrupt")

