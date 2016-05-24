import _cubetree

_lookup_tables_loaded = False

def gen_lookups():
    _cubetree.gen_corner_lookup()
    _cubetree.gen_upper_edge_lookup()
    _cubetree.gen_lower_edge_lookup()

def lookups_loaded():
    return _lookup_tables_loaded

def load_lookups():
    if not _lookup_tables_loaded:
        _cubetree.load_lookups()
    else:
        print("lookups already loaded")

