import time
import asyncio
import highfive

from . import model
from . import distribute


commands = dict()


def command(name):

    def register_command(f):
        commands[name] = f
        return f

    return register_command


@command("show")
async def show(cube, master):

    print()
    print(cube)


@command("help")
async def help(cube, master):

    print("""\
'exit' : Exits the program immediately.
'help' : Shows the command list.
'show' : Displays the cube in its current state.
'reset' : Resets the cube to a new, solved cube.
'shuffle' : Shuffles the cube a specified number of times from its current state.
            Providing a random seed makes the shuffle reproducible.
            Also prints the shuffle algorithm when finished.
'turn' : Performs a specified algorithm on the cube.
'solve' : Performs a distributed solve on the cube from its current state to the start state.
          The solution algorithm is printed when found.\
""")


@command("reset")
async def reset(cube, master):

    return model.Cube()


@command("shuffle")
async def shuffle(cube, master):

    shuffle_depth = None
    while shuffle_depth is None:

        try:

            shuffle_depth = int(input("shuffle depth: "))
            if shuffle_depth <= 0:
                shuffle_depth = None
                raise ValueError

        except ValueError:
            print("please enter a positive integer as the shuffle depth")

    seed = input("random seed (optional): ")
    if seed == "":
        shuffle_algorithm = cube.shuffle(shuffle_depth)
    else:
        shuffle_algorithm = cube.shuffle(shuffle_depth, seed)

    print("shuffle algorithm: {}".format(shuffle_algorithm))


@command("turn")
async def turn(cube, master):

    try:
        algorithm = model.Algorithm(input("algorithm: "))

    except ValueError:
        print("invalid algorithm")

    else:
        cube.apply_algorithm(algorithm)


@command("solve")
async def solve(cube, master):

    start_time = time.time()

    solution = await distribute.solve(cube, master)

    time_elapsed = int(time.time() - start_time)
    minutes_elapsed, seconds_elapsed = divmod(time_elapsed, 60)

    print("solution:", solution)
    print("solve took {}m{}s".format(minutes_elapsed, seconds_elapsed))


def run_command_loop(hostname, port):

    asyncio.get_event_loop().run_until_complete(command_loop(hostname, port))


async def command_loop(hostname, port):

    async with await highfive.start_master(
            host=hostname, port=port) as master:

        cube = model.Cube()

        print("type 'help' to view a list of commands\n")
        while True:

            command = input("> ")

            if command == "exit":
                break

            if command in commands:

                command_function = commands[command]

                new_cube = await command_function(cube, master)
                cube = new_cube if new_cube is not None else cube

            else:
                print("unknown command")

