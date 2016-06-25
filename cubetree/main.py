import sys
import getopt
import socket
import multiprocessing

import cubetree.cube
import cubetree.lookup
import cubetree.distribute


def main():

    try:
        opts, args = getopt.getopt(sys.argv[1:], "gtsw:h:p:", ["server", "workers=", "hostname=", "port="])
    except getopt.GetoptError as err:
        print("usage: python3 run.py [-g] [-t] [-s|--server] [-w|--workers=<num_workers>]")
        return

    gen = False
    test_solve = False
    serve = False
    start_workers = -1
    hostname = ""
    port = 48484

    for opt, arg in opts:
        if opt in ("-g", "--gen-lookups"):
            gen = True
        elif opt in ("-t", "--test-solve"):
            test_solve = True
        elif opt in ("-s", "--server"):
            serve = True
        elif opt in ("-w", "--workers"):
            start_workers = int(arg)
        elif opt in ("-h", "--host"):
            hostname = arg
        elif opt in ("-p", "--port"):
            port = int(arg)

    if gen:
        cubetree.lookup.load_or_gen_lookups()
    elif test_solve:
        cubetree.lookup.load_or_gen_lookups()
        c = cubetree.cube.Cube()
        c.shuffle(14)
        c.solve()
    else:
        if serve:
            solver = cubetree.distribute.DistributedSolver(hostname, port)
        if start_workers >= 0:
            cubetree.lookup.load_or_gen_lookups()
            if start_workers == 0:
                start_workers = multiprocessing.cpu_count()
            for i in range(start_workers):
                cubetree.distribute.start_worker(hostname, port)
        if serve:
            while True:
                shuffle_depth = int(input("shuffle: "))
                c = cubetree.cube.Cube()
                c.shuffle(shuffle_depth)
                print("SOLUTION:", solver.solve(c))
                if input("solve another? (y/n) ") != "y":
                    break

