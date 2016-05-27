import _cubetree

def gen_lookups():
    _cubetree.gen_corner_lookup()
    _cubetree.gen_upper_edge_lookup()
    _cubetree.gen_lower_edge_lookup()

def load_lookups():
    _cubetree.load_corner_lookup()
    _cubetree.load_upper_edge_lookup()
    _cubetree.load_lower_edge_lookup()

