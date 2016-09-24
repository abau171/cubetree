import time

import _cubetree


def profile_and_print():
    raw_tps = estimate_raw_tps()
    print("{} RAW MTPS".format(raw_tps / 1000000))
    ceps = estimate_ceps()
    print("{} MCEPS".format(ceps / 1000000))
    ueeps = estimate_ueeps()
    print("{} MUEEPS".format(ueeps / 1000000))
    leeps = estimate_leeps()
    print("{} MLEEPS".format(leeps / 1000000))
    eps = estimate_eps()
    print("{} MEPS".format(eps / 1000000))


def estimate_raw_tps():
    start_time = time.time()
    num_turns = 10000000
    _cubetree.run_cube_turns(num_turns)
    end_time = time.time()
    return num_turns / (end_time - start_time)


def estimate_ceps():
    start_time = time.time()
    num_encodes = 10000000
    _cubetree.run_corner_encodes(num_encodes)
    end_time = time.time()
    return num_encodes / (end_time - start_time)


def estimate_ueeps():
    start_time = time.time()
    num_encodes = 10000000
    _cubetree.run_upper_edge_encodes(num_encodes)
    end_time = time.time()
    return num_encodes / (end_time - start_time)


def estimate_leeps():
    start_time = time.time()
    num_encodes = 10000000
    _cubetree.run_lower_edge_encodes(num_encodes)
    end_time = time.time()
    return num_encodes / (end_time - start_time)


def estimate_eps():
    start_time = time.time()
    num_encodes = 10000000
    _cubetree.run_all_encodes(num_encodes)
    end_time = time.time()
    return num_encodes / (end_time - start_time)

