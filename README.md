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
