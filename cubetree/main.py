import sys
import getopt

from .cube import Cube
from .lookup import load_or_gen_lookups, load_corner_lookup, load_lower_edge_lookup, load_upper_edge_lookup

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "gt")
    except getopt.GetoptError as err:
        print("usage: python3 run.py [-g] [-t]")
        return
    for (opt, arg) in opts:
        if opt in ("-g", "--gen-lookups"):
            gen_lookups()
        elif opt in ("-t", "--test-solve"):
            load_or_gen_lookups()
            c = Cube()
            c.shuffle(14)
            c.solve()

