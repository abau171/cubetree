import enum
import random
import collections

import _cubetree

import cubetree.distribute.json_socket_proxy


_face_strings = ["U", "L", "F", "R", "B", "D"]
_tt_strings = ["?", "", "2", "'"]


class Face(enum.Enum):
    UP, LEFT, FRONT, RIGHT, BACK, DOWN = range(6)

    @classmethod
    def from_string(cls, face_str):
        return Face(_face_strings.index(face_str))

    @classmethod
    def to_string(cls, face):
        return _face_strings[face.value]

    def opposite(self):
        return [Face.DOWN, Face.RIGHT, Face.BACK, Face.LEFT, Face.FRONT, Face.UP][self.value]


class TurnType(enum.Enum):
    NO_TURN, CLOCKWISE, DOUBLE, COUNTER = range(4)

    @classmethod
    def from_string(cls, tt_str):
        return TurnType(_tt_strings.index(tt_str))

    @classmethod
    def to_string(cls, turn_type):
        return _tt_strings[turn_type.value]


def parse_move(move_str):
    if len(move_str) == 0:
        raise ValueError("Move string must contain either 1 or 2 characters.")
    face = Face.from_string(move_str[0])
    turn_type = TurnType.from_string(move_str[1:])
    return face, turn_type


class Algorithm(cubetree.distribute.json_socket_proxy.JSONSerializable):

    def __init__(self, move_list=None):
        if isinstance(move_list, str):
            self.move_list = list(parse_move(move_str) for move_str in move_list.split())
        elif isinstance(move_list, collections.Iterable):
            self.move_list = list(move_list)
        else:
            self.move_list = []

    def __str__(self):
        return " ".join(Face.to_string(face) + TurnType.to_string(turn_type) for face, turn_type in self.move_list)

    def __iter__(self):
        return iter(self.move_list)

    def __add__(self, other):
        return Algorithm(self.move_list + other.move_list)

    @classmethod
    def json_serialize(cls, obj):
        return [[face.value, turn_type.value] for face, turn_type in obj]

    @classmethod
    def json_deserialize(cls, obj):
        return Algorithm((Face(move[0]), TurnType(move[1])) for move in obj)


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

    def apply_algorithm(self, algorithm):
        for face, turn_type in algorithm:
            self.turn(face, turn_type)

    def shuffle(self, i):
        move_list = []
        last_face = None
        for _ in range(i):
            face = last_face
            while face == last_face:
                face = Face(random.randint(0, 5))
            last_face = face
            turn_type = TurnType(random.randint(1, 3))
            move_list.append((face, turn_type))
        shuffle_algorithm = Algorithm(move_list)
        self.apply_algorithm(shuffle_algorithm)
        return shuffle_algorithm

    def search_depth(self, depth, last_face=None):
        last_face_id = 6 if last_face is None else last_face.value
        raw_solution = self.raw_cube.search_depth(depth, last_face_id)
        if raw_solution is None:
            return None
        else:
            return Algorithm((Face(face_index), TurnType(turn_type_index)) for face_index, turn_type_index in raw_solution)

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


