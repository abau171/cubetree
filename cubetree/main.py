import sys
import getopt

from .lookup import gen_lookups

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "g")
    except getopt.GetoptError as err:
        print("usage: python3 run.py [-g]")
        return
    for (opt, arg) in opts:
        if opt in ("-g", "--gen-lookups"):
            gen_lookups()

