CubeTree
========
Optimal Rubik's cube solver built for collaborative distributed solving.

### About

CubeTree is a solver program for the popular 3-D combination puzzle, [Rubik's Cube](https://en.wikipedia.org/wiki/Rubik%27s_Cube).

Specifically, CubeTree is an implementation of [Korf's Algorithm](https://en.wikipedia.org/wiki/Optimal_solutions_for_Rubik%27s_Cube#Korf.27s_algorithm), meaning that CubeTree only produces optimal solutions (those which use the fewest possible number of turns). Most Rubik's Cube solver programs produce solutions in anywhere from a few minutes to a few milliseconds, however they cannot guarantee optimality of the solutions. CubeTree generally produces solutions to randomly scrambled cubes on a single decently powerful machine within several hours, but guarantees optimality.

Due to the large amount of processing power required to solve a Rubik's Cube optimally, CubeTree provides mechanisms for distributing the solution algorithm amongst an arbitrary number of computers.

CubeTree is written in Python 3 and uses custom C extensions for performance.

### Installation

1. Clone the repository, and `cd` into the project root.
2. (optional) Create a Python 3 virtual environment.
3. Finish the install with `pip install .`.

### Running

You can run CubeTree from the project root by running `python3 run.py`. This will simply print a usage dialog.

Generally, you will first want to generate important cache files by running `python3 run.py -g`. This may take several minutes, and the new cache files will be saved in the `cache/` directory. If you have multiple machines running CubeTree, the `cache/` directory can be copied to each to save you from having to generate them separately (though be warned, there are 166 MB of cache data to transfer!).

After the cache files have been generated, you can create a solve server and some workers.

To start a solve server, you can run `python3 -s`. This will drop you into a command prompt that lets you control the solving of cubes. Run `commands` to view a full list of commands.

The solve server alone cannot solve a cube, and must have at least one worker connected to it. To start a worker, you can run `python3 -w 0`. This will launch a worker for each (logical) core of your machine and connect to the local solve server. To connect a remote machine, run `python3 -w 0 -h <server address>`.

### Example

##### Server

The server always maintains a cube state which starts solved. You can `show` the state, or manipulate it using `shuffle` and providing a number of random turns to make or `turn` and providing an algorithm (these are both shown below).

You can then run `solve` to offload solving work to the connected workers. After the solve completes, an algorithm is printed but the cube state is not changed. If you want to verify the solve, you can simply run `turn` then copy the algorithm produced by the `solve` command (this is also shown below).

When done, simply run `exit`.

```
$ pr -s
type 'commands' to view a list of commands

> show

     0 0 0
     0 0 0
     0 0 0
1 1 12 2 23 3 34 4 4
1 1 12 2 23 3 34 4 4
1 1 12 2 23 3 34 4 4
     5 5 5
     5 5 5
     5 5 5

> shuffle
shuffle depth: 14
shuffle algorithm: L' U' D B2 F' L U' L' U' D' B F2 R2 B'
> show

     5 3 3
     0 0 0
     0 5 1
4 2 34 2 20 3 25 2 1
3 1 41 2 45 3 10 4 5
0 5 12 4 34 3 20 1 4
     5 0 5
     1 5 4
     1 2 3

> solve
DEPTH 1
DEPTH 2
DEPTH 3
DEPTH 4
DEPTH 5
DEPTH 6
DEPTH 7
DEPTH 8
DEPTH 9
DEPTH 10
DEPTH 11
DEPTH 12
DEPTH 13
DEPTH 14
solution: B R2 F2 B' U D L U L' F B2 U D' L
solve took 0m33s
> turn
algorithm: B R2 F2 B' U D L U L' F B2 U D' L
> show

     0 0 0
     0 0 0
     0 0 0
1 1 12 2 23 3 34 4 4
1 1 12 2 23 3 34 4 4
1 1 12 2 23 3 34 4 4
     5 5 5
     5 5 5
     5 5 5

> exit
```

##### Worker

Workers are much more passive, and simply wait for instruction from the solve server they connected to. They recieve sub-tasks from the solve server to run, and print a single `.` if a sub-task did not yield a solution or an `X` if it did. More sub-tasks are generated during deeper solves, so there will generally be a very large number of `.`s, and very few `X`s. In fact, if multiple machines are running together, most likely only one will find a solution and print an `X`.

```
python3 run.py -w 0
corner lookup loaded from file
upper edge lookup loaded from file
lower edge lookup loaded from file
.............X
```
