#include <stdlib.h>

#include <cube.h>
#include <lookup.h>

#include <profile.h>

// Turn cube a number of times.
void run_cube_turns(int num_turns) {
    cube_t cube = solved_cube;
    for (int i = 0; i < num_turns; i++) {
        turnCubeSelf(&cube, rand() % 6, rand() % 3 + 1);
    }
}

// Encode a cube's corners a number of times.
void run_cube_corner_encodes(int num_encodes)
{
    cube_t cube = solved_cube;
    for (int i = 0; i < num_encodes; i++) {
        turnCubeSelf(&cube, rand() % 6, rand() % 3 + 1);
        encodeCornerSystem(&cube.cornersystem);
    }
}

// Encode a cube's upper edges a number of times.
void run_cube_upper_edge_encodes(int num_encodes)
{
    cube_t cube = solved_cube;
    for (int i = 0; i < num_encodes; i++) {
        turnCubeSelf(&cube, rand() % 6, rand() % 3 + 1);
        encodeUpperEdgeSystem(&cube.edgesystem);
    }
}

// Encode a cube's lower edges a number of times.
void run_cube_lower_edge_encodes(int num_encodes)
{
    cube_t cube = solved_cube;
    for (int i = 0; i < num_encodes; i++) {
        turnCubeSelf(&cube, rand() % 6, rand() % 3 + 1);
        encodeLowerEdgeSystem(&cube.edgesystem);
    }
}

// Encode an entire cube a number of times.
void run_cube_all_encodes(int num_encodes)
{
    cube_t cube = solved_cube;
    for (int i = 0; i < num_encodes; i++) {
        turnCubeSelf(&cube, rand() % 6, rand() % 3 + 1);
        encodeCornerSystem(&cube.cornersystem);
        encodeUpperEdgeSystem(&cube.edgesystem);
        encodeLowerEdgeSystem(&cube.edgesystem);
    }
}


