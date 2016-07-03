import sys
import getopt
import socket
import multiprocessing

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
        cubetree.distribute.server.run_solver(hostname, port)
    elif action == "work":
        cubetree.distribute.worker.run_worker(hostname, port, num_workers)
    else:
        print_usage()

