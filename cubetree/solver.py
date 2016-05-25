import enum

import _cubetree

class Face(enum.Enum):
    UP, LEFT, FRONT, RIGHT, BACK, DOWN = range(6)


class TurnType(enum.Enum):
    NO_TURN, CLOCKWISE, DOUBLE, COUNTER = range(4)


class Move:

    def __init__(self, face, turn_type):
        self.face = face
        self.turn_type = turn_type

    def __str__(self):
        return self.face.name + " " + self.turn_type.name

def search_depth(cube, depth):
    raw_solution = cube.raw_cube.search_depth(depth)
    if raw_solution == None:
        return None
    return [Move(Face(face_index), TurnType(turn_type_index)) for face_index, turn_type_index in raw_solution]

def solve(cube):
    if cube.is_solved():
        return []
    cur_depth = 0
    while True:
        cur_depth += 1
        possible_solution = search_depth(cube, cur_depth)
        if possible_solution != None:
            return possible_solution

def cancel_checker():
    return False

_cubetree.set_cancel_checker(cancel_checker)

