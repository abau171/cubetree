#ifndef CUBE_UTIL_H
#define CUBE_UTIL_H

#define positive_modulo(x, mod) (((x % mod) + mod) % mod)

#define U_FACE 0
#define L_FACE 1
#define F_FACE 2
#define R_FACE 3
#define B_FACE 4
#define D_FACE 5

#define CLOCKWISE_TURN 1
#define DOUBLE_TURN 2
#define COUNTER_TURN 3

#endif
