import enum
import random
import collections

import _cubetree


face_chars = ["U", "L", "F", "R", "B", "D"]


class Face(enum.Enum):

    UP, LEFT, FRONT, RIGHT, BACK, DOWN = range(6)

    @classmethod
    def from_char(cls, face_str):

        return Face(face_chars.index(face_str))

    @classmethod
    def random(cls):

        return Face(random.randint(0, 5))

    def __init__(self, value):

        self.char = face_chars[value]

    def opposite(self):

        return face_opposites[self.value]


face_opposites = [Face.DOWN, Face.RIGHT, Face.BACK, Face.LEFT, Face.FRONT, Face.UP]


tt_chars = ["?", "", "2", "'"]


class TurnType(enum.Enum):

    NO_TURN, CLOCKWISE, DOUBLE, COUNTER = range(4)

    @classmethod
    def from_char(cls, tt_str):

        return TurnType(tt_chars.index(tt_str))

    @classmethod
    def random(self):

        return TurnType(random.randint(1, 3))

    def __init__(self, value):

        self.char = tt_chars[value]


def ensure_move(m):

    if isinstance(m, str):

        if len(m) == 0:
            raise ValueError("Move string must contain either 1 or 2 characters.")

        face = Face.from_char(m[0])
        turn_type = TurnType.from_char(m[1:])

        return [face, turn_type]

    elif isinstance(m[0], int):
        return [Face(m[0]), TurnType(m[1])]

    else:
        return m


class Algorithm:

    def __init__(self, move_list=None):

        if isinstance(move_list, str):
            self._move_list = list(ensure_move(move_str) for move_str in move_list.split())

        elif isinstance(move_list, collections.Iterable):
            self._move_list = [ensure_move(m) for m in move_list]

        else:
            self._move_list = []

    def __str__(self):

        return " ".join(face.char + turn_type.char
            for face, turn_type in self._move_list)

    def __len__(self):

        return len(self._move_list)

    def __getitem__(self, i):

        return self._move_list[i]

    def __iter__(self):

        return iter(self._move_list)

    def __add__(self, other):

        return Algorithm(self._move_list + other._move_list)

    def get_state(self):

        return [[face.value, turn_type.value] for face, turn_type
                in self._move_list]


class Cube:

    def __init__(self, state=None):

        if state is None:
            self._raw_cube = _cubetree.Cube()

        else:
            self._raw_cube = _cubetree.Cube(state)

    def _row_str(self, face, row):

        if row == 0:
            return "{} {} {}".format(*(self._raw_cube.get_facelet(face, i)
                for i in range(3)))

        elif row == 1:
            return "{} {} {}".format(self._raw_cube.get_facelet(face, 7), face,
                self._raw_cube.get_facelet(face, 3))

        else:
            return "{} {} {}".format(*(self._raw_cube.get_facelet(face, i)
                for i in range(6, 3, -1)))

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

        return self._raw_cube.get_state()

    def is_solved(self):

        return self._raw_cube.is_solved()

    def turn(self, face, turn_type):

        self._raw_cube.turn(face.value, turn_type.value)

    def apply_algorithm(self, algorithm):

        for face, turn_type in algorithm:
            self.turn(face, turn_type)

    def shuffle(self, i, seed=None):

        if seed is not None:
            rand_state = random.getstate()
            random.seed(seed)

        move_list = []
        last_face = None

        for _ in range(i):

            face = last_face
            while face == last_face:
                face = Face.random()

            turn_type = TurnType.random()

            move_list.append((face, turn_type))

            last_face = face

        shuffle_algorithm = Algorithm(reversed(move_list))
        self.apply_algorithm(shuffle_algorithm)

        if seed is not None:
            random.setstate(rand_state)

        return shuffle_algorithm

    def search_depth(self, depth, last_face=None):

        last_face_id = 6 if last_face is None else last_face.value
        raw_solution = self._raw_cube.search_depth(depth, last_face_id)

        if raw_solution is None:
            return None

        else:
            return Algorithm(raw_solution)

    def solve(self):

        if self.is_solved():
            return Algorithm()

        cur_depth = 0

        while True:

            cur_depth += 1
            print("DEPTH {}".format(cur_depth))

            possible_solution = self.search_depth(cur_depth)

            if possible_solution != None:
                return possible_solution

    def clone(self):

        return Cube(self.get_state())

