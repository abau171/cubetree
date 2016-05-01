import _cubetree

class Cube:
    def __init__(self):
        self.raw_cube = _cubetree.Cube()
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
    def is_solved(self):
        return self.raw_cube.is_solved()
    def shuffle(self, i):
        self.raw_cube.shuffle(i)
    def load_lookups(self):
        self.raw_cube.load_lookups()
    def search_depth(self, depth):
        self.raw_cube.search_depth(depth)

