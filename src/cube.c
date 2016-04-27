#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <cube.h>

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

const cornersystem_t solved_cornersystem = { .corners={
    { .cid=0, .rotation=0 }, { .cid=1, .rotation=0 }, { .cid=2, .rotation=0 },
    { .cid=3, .rotation=0 }, { .cid=4, .rotation=0 }, { .cid=5, .rotation=0 },
    { .cid=6, .rotation=0 }, { .cid=7, .rotation=0 }
}};

const edgesystem_t solved_edgesystem = { .edges={
    { .eid=0, .flip=0 }, { .eid=1, .flip=0 }, { .eid=2, .flip=0 },
    { .eid=3, .flip=0 }, { .eid=4, .flip=0 }, { .eid=5, .flip=0 },
    { .eid=6, .flip=0 }, { .eid=7, .flip=0 }, { .eid=8, .flip=0 },
    { .eid=9, .flip=0 }, { .eid=10, .flip=0 }, { .eid=11, .flip=0 }
}};

static const corner_t face_corners[6][4] = {
    {
        { .cid=2, .rotation=0},
        { .cid=3, .rotation=0},
        { .cid=0, .rotation=0},
        { .cid=1, .rotation=0}
    },
    {
        { .cid=2, .rotation=1},
        { .cid=1, .rotation=2},
        { .cid=5, .rotation=1},
        { .cid=6, .rotation=2}
    },
    {
        { .cid=1, .rotation=1},
        { .cid=0, .rotation=2},
        { .cid=4, .rotation=1},
        { .cid=5, .rotation=2}
    },
    {
        { .cid=0, .rotation=1},
        { .cid=3, .rotation=2},
        { .cid=7, .rotation=1},
        { .cid=4, .rotation=2}
    },
    {
        { .cid=3, .rotation=1},
        { .cid=2, .rotation=2},
        { .cid=6, .rotation=1},
        { .cid=7, .rotation=2}
    },
    {
        { .cid=5, .rotation=0},
        { .cid=4, .rotation=0},
        { .cid=7, .rotation=0},
        { .cid=6, .rotation=0}
    }
};

static const uint8_t corner_facelets[8][3] = {
    {U_FACE, R_FACE, F_FACE},
    {U_FACE, F_FACE, L_FACE},
    {U_FACE, L_FACE, B_FACE},
    {U_FACE, B_FACE, R_FACE},
    {D_FACE, F_FACE, R_FACE},
    {D_FACE, L_FACE, F_FACE},
    {D_FACE, B_FACE, L_FACE},
    {D_FACE, R_FACE, B_FACE}
};

static const edge_t face_edges[6][4] = {
    {
        { .eid=2, .flip=0},
        { .eid=3, .flip=0},
        { .eid=0, .flip=0},
        { .eid=1, .flip=0}
    },
    {
        { .eid=1, .flip=1},
        { .eid=5, .flip=1},
        { .eid=9, .flip=1},
        { .eid=6, .flip=1}
    },
    {
        { .eid=0, .flip=1},
        { .eid=4, .flip=0},
        { .eid=8, .flip=1},
        { .eid=5, .flip=0}
    },
    {
        { .eid=3, .flip=1},
        { .eid=7, .flip=1},
        { .eid=11, .flip=1},
        { .eid=4, .flip=1}
    },
    {
        { .eid=2, .flip=1},
        { .eid=6, .flip=0},
        { .eid=10, .flip=1},
        { .eid=7, .flip=0}
    },
    {
        { .eid=8, .flip=0},
        { .eid=11, .flip=0},
        { .eid=10, .flip=0},
        { .eid=9, .flip=0}
    }
};

static uint8_t edge_facelets[12][2] = {
    {U_FACE, F_FACE},
    {U_FACE, L_FACE},
    {U_FACE, B_FACE},
    {U_FACE, R_FACE},
    {F_FACE, R_FACE},
    {F_FACE, L_FACE},
    {B_FACE, L_FACE},
    {B_FACE, R_FACE},
    {D_FACE, F_FACE},
    {D_FACE, L_FACE},
    {D_FACE, B_FACE},
    {D_FACE, R_FACE}
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

void turnCornerSystem(cornersystem_t* destination,
                      const cornersystem_t* source,
                      uint8_t face,
                      int turn_type) {
    *destination = *source;
    for (int i = 0; i < 4; i++) {
        int j = (i + turn_type) % 4;
        const corner_t* start_face_corner = &face_corners[face][i];
        const corner_t* end_face_corner = &face_corners[face][j];
        const corner_t* cur_corner = &source->corners[start_face_corner->cid];
        uint8_t new_rotation = positive_modulo(cur_corner->rotation + end_face_corner->rotation - start_face_corner->rotation, 3);
        destination->corners[end_face_corner->cid].cid = cur_corner->cid;
        destination->corners[end_face_corner->cid].rotation = new_rotation;
    }
}

void turnEdgeSystem(edgesystem_t* destination,
                    const edgesystem_t* source,
                    uint8_t face,
                    int turn_type) {
    *destination = *source;
    for (int i = 0; i < 4; i++) {
        int j = (i + turn_type) % 4;
        const edge_t* start_face_edge = &face_edges[face][i];
        const edge_t* end_face_edge = &face_edges[face][j];
        const edge_t* cur_edge = &source->edges[start_face_edge->eid];
        uint8_t new_flip = positive_modulo(cur_edge->flip + end_face_edge->flip - start_face_edge->flip, 2);
        destination->edges[end_face_edge->eid].eid = cur_edge->eid;
        destination->edges[end_face_edge->eid].flip = new_flip;
    }
}

uint8_t getFaceletCube(const cube_t* cube, uint8_t face, uint8_t i) {
    if (i % 2 == 0) {
        return getFaceletCornerSystem(&cube->cornersystem, face, i / 2);
    } else {
        return getFaceletEdgeSystem(&cube->edgesystem, face, (i - 1) / 2);
    }
}

uint8_t getFaceletCornerSystem(const cornersystem_t* cs, uint8_t face, int i) {
    const corner_t* face_corner = &face_corners[face][i];
    const corner_t* corner = &cs->corners[face_corner->cid];
    uint8_t facelet_rotation = positive_modulo(face_corner->rotation - corner->rotation, 3);
    return corner_facelets[corner->cid][facelet_rotation];
}

uint8_t getFaceletEdgeSystem(const edgesystem_t* es, uint8_t face, int i) {
    const edge_t* face_edge = &face_edges[face][i];
    const edge_t* edge = &es->edges[face_edge->eid];
    uint8_t facelet_flip = positive_modulo(face_edge->flip - edge->flip, 2);
    return edge_facelets[edge->eid][facelet_flip];
}

bool isSolvedCube(const cube_t* cube) {
    return isSolvedCornerSystem(&cube->cornersystem) && isSolvedEdgeSystem(&cube->edgesystem);
}

bool isSolvedCornerSystem(const cornersystem_t* cs) {
    for (int i = 0; i < 8; i++) {
        if (cs->corners[i].cid != i || cs->corners[i].rotation != 0) {
            return false;
        }
    }
    return true;
}

bool isSolvedEdgeSystem(const edgesystem_t* es) {
    for (int i = 0; i < 12; i++) {
        if (es->edges[i].eid != i || es->edges[i].flip != 0) {
            return false;
        }
    }
    return true;
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

static char face_chars[] = {'U', 'L', 'F', 'R', 'B', 'D'};

static char turn_chars[] = {'?', ' ', '2', '\''};

void printCubeMove(uint8_t face, int turn_type) {
    putchar(face_chars[face]);
    putchar(turn_chars[turn_type]);
    putchar('\n');
}

void printCornerSystem(const cornersystem_t* cs) {
    for (int i = 0; i < 8; i++) {
        printf("%d, %d\n", cs->corners[i].cid, cs->corners[i].rotation);
    }
    for (uint8_t face = 0; face < 6; face++) {
        for (int i = 0; i < 4; i++) {
            printf("%d", getFaceletCornerSystem(cs, face, i));
        }
        putchar('\n');
    }
}

void printEdgeSystem(const edgesystem_t* es) {
    for (int i = 0; i < 12; i++) {
        printf("%d, %d\n", es->edges[i].eid, es->edges[i].flip);
    }
    for (uint8_t face = 0; face < 6; face++) {
        for (int i = 0; i < 4; i++) {
            printf("%d", getFaceletEdgeSystem(es, face, i));
        }
        putchar('\n');
    }
}

