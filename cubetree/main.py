import sys
import getopt
import socket
import multiprocessing

from .cube import Cube, format_move_list
from .lookup import load_or_gen_lookups, load_corner_lookup, load_lower_edge_lookup, load_upper_edge_lookup
from .server import DistributedSolver
from .worker import start_worker

def main():

    try:
        opts, args = getopt.getopt(sys.argv[1:], "gtsw:", ["server", "workers="])
    except getopt.GetoptError as err:
        print("usage: python3 run.py [-g] [-t] [-s|--server] [-w|--workers=<num_workers>]")
        return

    gen = False
    test_solve = False
    serve = False
    start_workers = -1

    for opt, arg in opts:
        if opt in ("-g", "--gen-lookups"):
            gen = True
        elif opt in ("-t", "--test-solve"):
            test_solve = True
        elif opt in ("-s", "--server"):
            serve = True
        elif opt in ("-w", "--workers"):
            start_workers = int(arg)

    if gen:
        gen_lookups()
    elif test_solve:
        load_or_gen_lookups()
        c = Cube()
        c.shuffle(14)
        c.solve()
    else:
        if serve:
            solver = DistributedSolver(48484)
        if start_workers >= 0:
            load_or_gen_lookups()
            num_workers = int(arg)
            if num_workers == 0:
                num_workers = multiprocessing.cpu_count()
            for i in range(num_workers):
                start_worker(socket.gethostname(), 48484)
        if serve:
            while True:
                shuffle_depth = int(input("shuffle: "))
                c = Cube()
                c.shuffle(shuffle_depth)
                print(format_move_list(solver.solve(c)))

