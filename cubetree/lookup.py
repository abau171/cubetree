import _cubetree

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
    if not _cubetree.save_corner_lookup():
        raise LookupSaveFailedException

def save_upper_edge_lookup():
    if not _cubetree.save_upper_edge_lookup():
        raise LookupSaveFailedException

def save_lower_edge_lookup():
    if not _cubetree.save_lower_edge_lookup():
        raise LookupSaveFailedException

