import os

import _cubetree


def _ensure_cache_directory():
    if not os.path.isdir("cache/"):
        os.mkdir("cache/")


def load_or_gen_corner():

    if _cubetree.load_corner_lookup():

        print("corner lookup loaded from file")

    else:

        print("generating corner lookup...")

        _cubetree.gen_corner_lookup()

        _ensure_cache_directory()
        if _cubetree.save_corner_lookup():
            print("corner lookup file saved")
        else:
            raise OSError("could not save corner lookup file")


def load_or_gen_upper_edge():

    if _cubetree.load_upper_edge_lookup():

        print("upper edge lookup loaded from file")

    else:

        print("generating upper edge lookup...")

        _cubetree.gen_upper_edge_lookup()

        _ensure_cache_directory()
        if _cubetree.save_upper_edge_lookup():
            print("upper edge lookup file saved")
        else:
            raise OSError("could not save upper edge lookup file")


def load_or_gen_lower_edge():

    if _cubetree.load_lower_edge_lookup():

        print("lower edge lookup loaded from file")

    else:

        print("generating lower edge lookup...")

        _cubetree.gen_lower_edge_lookup()

        _ensure_cache_directory()
        if _cubetree.save_lower_edge_lookup():
            print("lower edge lookup file saved")
        else:
            raise OSError("could not save lower edge lookup file")


def load_or_gen_lookups():

    load_or_gen_corner()
    load_or_gen_upper_edge()
    load_or_gen_lower_edge()

