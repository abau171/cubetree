#ifndef A_STAR_H
#define A_STAR_H

#include <stdbool.h>

#include <cube.h>

/* linked list node for chains of cube turns */
typedef struct movenode_t {
    uint8_t face;
    int turn_type;
    struct movenode_t* next_node;
} movenode_t;

void set_cancel_checker(bool (*checker)(void));

/* search the cube for a solution at the given depth */
movenode_t* searchDepth(const cube_t* last_cube, int depth);

#endif
