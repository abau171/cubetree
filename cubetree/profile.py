import time

import _cubetree


def ops_per_second(f, num_ops):

    start_time = time.time()

    f(num_ops)

    end_time = time.time()
    return num_ops / (end_time - start_time)


def profile_and_print():

    n = 5000000

    raw_tps = ops_per_second(_cubetree.run_cube_turns, n)
    print("{} RAW MTPS".format(raw_tps / 1000000))

    ceps = ops_per_second(_cubetree.run_corner_encodes, n)
    print("{} MCEPS".format(ceps / 1000000))

    ueeps = ops_per_second(_cubetree.run_upper_edge_encodes, n)
    print("{} MUEEPS".format(ueeps / 1000000))

    leeps = ops_per_second(_cubetree.run_lower_edge_encodes, n)
    print("{} MLEEPS".format(leeps / 1000000))

    eps = ops_per_second(_cubetree.run_all_encodes, n)
    print("{} MEPS".format(eps / 1000000))

