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

    def __repr__(self):
        return "Move({}, {})".format(str(self.face), str(self.turn_type))


def encode_move_list(l):
    return [[move.face.value, move.turn_type.value] for move in l]


def decode_move_list(l):
    return [Move(Face(move[0]), TurnType(move[1])) for move in l]


class Cube:

    def __init__(self, state=None):
        
        if state is None:
            self.raw_cube = _cubetree.Cube()
        else:
            self.raw_cube = _cubetree.Cube(state)

    def _row_str(self, face, row):
        if row == 0:
            return "{} {} {}".format(*(self.raw_cube.get_facelet(face, i) for i in range(3)))
        elif row == 1:
            return "{} {} {}".format(self.raw_cube.get_facelet(face, 7), face, self.raw_cube.get_facelet(face, 3))
        else:
            return "{} {} {}".format(*(self.raw_cube.get_facelet(face, i) for i in range(6, 3, -1)))

    def __str__(self):
        result = ""
        for row in range(3):
            result += "     " + self._row_str(0, row) + "\n"
        for row in range(3):
            for face in range(1, 5):
                result += self._row_str(face, row)
            result += "\n"
        for row in range(3):
            result += "     " + self._row_str(5, row) + "\n"
        return result

    def get_state(self):
        return self.raw_cube.get_state()

    def is_solved(self):
        return self.raw_cube.is_solved()

    def turn(self, face, turn_type):
        self.raw_cube.turn(face.value, turn_type.value)

    def shuffle(self, i):
        self.raw_cube.shuffle(i)

    def search_depth(self, depth):
        raw_solution = self.raw_cube.search_depth(depth)
        if raw_solution is None:
            return None
        else:
            return [Move(Face(face_index), TurnType(turn_type_index)) for face_index, turn_type_index in raw_solution]

    def solve(self):
        if self.is_solved():
            return []
        cur_depth = 0
        while True:
            cur_depth += 1
            print("DEPTH {}".format(cur_depth))
            possible_solution = self.search_depth(cur_depth)
            if possible_solution != None:
                return possible_solution


