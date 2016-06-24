import os

import _cubetree


def _ensure_cache_directory():
    if not os.path.isdir("cache/"):
        os.mkdir("cache/")


class LookupLoadFailedException(Exception):
    pass


class LookupSaveFailedException(Exception):
    pass


def load_corner_lookup():
    if not _cubetree.load_corner_lookup():
        raise LookupLoadFailedException


def load_upper_edge_lookup():
    if not _cubetree.load_upper_edge_lookup():
        raise LookupLoadFailedException


def load_lower_edge_lookup():
    if not _cubetree.load_lower_edge_lookup():
        raise LookupLoadFailedException


def gen_corner_lookup():
    _cubetree.gen_corner_lookup()


def gen_upper_edge_lookup():
    _cubetree.gen_upper_edge_lookup()


def gen_lower_edge_lookup():
    _cubetree.gen_lower_edge_lookup()


def save_corner_lookup():
    _ensure_cache_directory()
    if not _cubetree.save_corner_lookup():
        raise LookupSaveFailedException


def save_upper_edge_lookup():
    _ensure_cache_directory()
    if not _cubetree.save_upper_edge_lookup():
        raise LookupSaveFailedException


def save_lower_edge_lookup():
    _ensure_cache_directory()
    if not _cubetree.save_lower_edge_lookup():
        raise LookupSaveFailedException


def load_or_gen_lookups():

    try:
        load_corner_lookup()
        print("corner lookup loaded from file")
    except LookupLoadFailedException:
        print("generating corner lookup...")
        gen_corner_lookup()
        save_corner_lookup()

    try:
        load_upper_edge_lookup()
        print("upper edge lookup loaded from file")
    except LookupLoadFailedException:
        print("generating upper edge lookup...")
        gen_upper_edge_lookup()
        save_upper_edge_lookup()

    try:
        load_lower_edge_lookup()
        print("lower edge lookup loaded from file")
    except LookupLoadFailedException:
        print("generating lower edge lookup...")
        gen_lower_edge_lookup()
        save_lower_edge_lookup()

