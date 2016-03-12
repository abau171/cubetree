#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <cube_utils.h>
#include <cube.h>
#include <corner.h>
#include <edge.h>

const cube_t solved_cube = {
    .cornersystem={ .corners={
        { .cid=0, .rotation=0 }, { .cid=1, .rotation=0 }, { .cid=2, .rotation=0 },
        { .cid=3, .rotation=0 }, { .cid=4, .rotation=0 }, { .cid=5, .rotation=0 },
        { .cid=6, .rotation=0 }, { .cid=7, .rotation=0 }
    }},
    .edgesystem={ .edges={
        { .eid=0, .flip=0 }, { .eid=1, .flip=0 }, { .eid=2, .flip=0 },
        { .eid=3, .flip=0 }, { .eid=4, .flip=0 }, { .eid=5, .flip=0 },
        { .eid=6, .flip=0 }, { .eid=7, .flip=0 }, { .eid=8, .flip=0 },
        { .eid=9, .flip=0 }, { .eid=10, .flip=0 }, { .eid=11, .flip=0 }
    }}
};

void turnCube(cube_t* destination,
              const cube_t* source,
              uint8_t face,
              int turn_type) {
    turnCornerSystem(&destination->cornersystem,
                     &source->cornersystem,
                     face,
                     turn_type);
    turnEdgeSystem(&destination->edgesystem,
                   &source->edgesystem,
                   face,
                   turn_type);
}

void turnCubeSelf(cube_t* cube,
                  uint8_t face,
                  int turn_type) {
    cube_t original_cube = *cube;
    turnCube(cube, &original_cube, face, turn_type);
}

uint8_t getFaceletCube(const cube_t* cube, uint8_t face, uint8_t i) {
    if (i % 2 == 0) {
        return getFaceletCornerSystem(&cube->cornersystem, face, i / 2);
    } else {
        return getFaceletEdgeSystem(&cube->edgesystem, face, (i - 1) / 2);
    }
}

bool isSolvedCube(const cube_t* cube) {
    return isSolvedCornerSystem(&cube->cornersystem) && isSolvedEdgeSystem(&cube->edgesystem);
}

void shuffleCube(cube_t* cube, int iterations) {
    srand(time(NULL));
    uint8_t last_face = 6;
    for (int i = 0; i < iterations; i++) {
        uint8_t face;
        do {
            face = rand() % 6;
        } while (face == last_face);
        int turn_type = rand() % 3 + 1;
        printCubeMove(face, turn_type);
        turnCubeSelf(cube, face, turn_type);
        last_face = face;
    }
}

void printCube(const cube_t* cube) {
    for (int face = 0; face < 6; face++) {
        for (int i = 0; i < 8; i++) {
            printf("%d", getFaceletCube(cube, face, i));
        }
        putchar('\n');
    }
}
