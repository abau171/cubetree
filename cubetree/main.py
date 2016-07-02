import sys
import getopt
import socket
import multiprocessing
import time

import cubetree.cube
import cubetree.lookup
import cubetree.distribute.server
import cubetree.distribute.worker
import cubetree.profile


def print_usage():
    print("""\
usage: python3 run.py [-g|--gen-lookups]
       python3 run.py [-s|--server] [-h|--hostname=<hostname>] [-p|--port=<port>]
       python3 run.py [-w|--workers=<num_workers>] [-h|--hostname=<hostname>] [-p|--port=<port>]
       python3 run.py [-P|--profile]\
""")


def main():

    try:
        opts, args = getopt.getopt(sys.argv[1:], "gsw:h:p:P", ["server", "workers=", "hostname=", "port=", "profile"])
    except getopt.GetoptError as err:
        print_usage()
        return

    action = None
    num_workers = None
    hostname = ""
    port = 48484

    for opt, arg in opts:
        if opt in ("-g", "--gen-lookups"):
            action = "gen"
        elif opt in ("-s", "--server"):
            action = "serve"
        elif opt in ("-w", "--workers"):
            action = "work"
            num_workers = int(arg)
        elif opt in ("-h", "--host"):
            hostname = arg
        elif opt in ("-p", "--port"):
            port = int(arg)
        elif opt in ("-P", "--profile"):
            action = "profile"

    if action == "gen":
        cubetree.lookup.load_or_gen_lookups()
    elif action == "profile":
        cubetree.profile.profile_and_print()
    elif action == "serve":
        solver = cubetree.distribute.server.DistributedSolver(hostname, port)
        while True:
            command = input("\nshuffle depth or 'exit': ")
            if command == "exit":
                break
            try:
                shuffle_depth = int(command)
            except ValueError:
                print("please enter an integer as the shuffle depth")
                continue
            c = cubetree.cube.Cube()
            shuffle_algorithm = c.shuffle(shuffle_depth)
            print("\nshuffle used: {}\n".format(shuffle_algorithm))
            print(c)
            start_time = time.time()
            print("solution:", solver.solve(c))
            time_elapsed = time.time() - start_time
            seconds_elapsed = int(time_elapsed % 60)
            minutes_elapsed = int(time_elapsed // 60)
            print("solve took {}m{}s".format(minutes_elapsed, seconds_elapsed))
    elif action == "work":
        cubetree.lookup.load_or_gen_lookups()
        if num_workers == 0:
            num_workers = multiprocessing.cpu_count()
        for i in range(num_workers):
            cubetree.distribute.worker.start_worker(hostname, port)
        try:
            cubetree.distribute.worker.join_workers()
        except KeyboardInterrupt:
            print("terminating workers...")
            cubetree.distribute.worker.terminate_workers()
    else:
        print_usage()
        return

