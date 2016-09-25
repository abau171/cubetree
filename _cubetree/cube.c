#include <stdlib.h>

#include <cube.h>

/* Lookup Tables for Manipulating and Observing Cube */

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

/* Functions for Manipulating Cube */

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
    // for each corner in the rotated face
    for (int i = 0; i < 4; i++) {
        // index of new corner position
        int j = (i + turn_type) % 4;
        // start corner orientation
        const corner_t* start_face_corner = &face_corners[face][i];
        // end corner orientation
        const corner_t* end_face_corner = &face_corners[face][j];
        // physical corner being moved between orientations
        const corner_t* cur_corner = &source->corners[start_face_corner->cid];
        // rotate by the difference between start and end orientation rotations
        uint8_t new_rotation = positive_modulo(cur_corner->rotation + end_face_corner->rotation - start_face_corner->rotation, 3);
        // set the corner id at the end orientation to the rotated corner's id
        destination->corners[end_face_corner->cid].cid = cur_corner->cid;
        // update the rotation for the rotated corner
        destination->corners[end_face_corner->cid].rotation = new_rotation;
    }
}

void turnEdgeSystem(edgesystem_t* destination,
                    const edgesystem_t* source,
                    uint8_t face,
                    int turn_type) {
    *destination = *source;
    // for each edge in the rotated face
    for (int i = 0; i < 4; i++) {
        // index of new edge position
        int j = (i + turn_type) % 4;
        // start edge orientation
        const edge_t* start_face_edge = &face_edges[face][i];
        // end edge orientation
        const edge_t* end_face_edge = &face_edges[face][j];
        // physical edge being moved between orientations
        const edge_t* cur_edge = &source->edges[start_face_edge->eid];
        // flip the edge if the start and end edges are flipped differently
        uint8_t new_flip = positive_modulo(cur_edge->flip + end_face_edge->flip - start_face_edge->flip, 2);
        // set the edge id at the end orientation to the rotated edge's id
        destination->edges[end_face_edge->eid].eid = cur_edge->eid;
        // update the flip for the rotated edge
        destination->edges[end_face_edge->eid].flip = new_flip;
    }
}

/* Functions for Observing Cube */

uint8_t getFaceletCube(const cube_t* cube, uint8_t face, uint8_t i) {
    // even indices are for corners, odd indices are for edges
    if (i % 2 == 0) {
        return getFaceletCornerSystem(&cube->cornersystem, face, i / 2);
    } else {
        return getFaceletEdgeSystem(&cube->edgesystem, face, (i - 1) / 2);
    }
}

uint8_t getFaceletCornerSystem(const cornersystem_t* cs, uint8_t face, int i) {
    // find the corner orientation
    const corner_t* face_corner = &face_corners[face][i];
    // find the physical corner at the orientation
    const corner_t* corner = &cs->corners[face_corner->cid];
    // find the inverse rotation of the physical corner
    uint8_t facelet_rotation = positive_modulo(face_corner->rotation - corner->rotation, 3);
    // return the facelet at the found rotation
    return corner_facelets[corner->cid][facelet_rotation];
}

uint8_t getFaceletEdgeSystem(const edgesystem_t* es, uint8_t face, int i) {
    // find the edge orientation
    const edge_t* face_edge = &face_edges[face][i];
    // find the physical edge at the orientation
    const edge_t* edge = &es->edges[face_edge->eid];
    // find the inverse flip of the physical edge
    uint8_t facelet_flip = positive_modulo(face_edge->flip - edge->flip, 2);
    // return the facelet at the found flip
    return edge_facelets[edge->eid][facelet_flip];
}

bool isSolvedCube(const cube_t* cube) {
    return isSolvedCornerSystem(&cube->cornersystem) && isSolvedEdgeSystem(&cube->edgesystem);
}

bool isSolvedCornerSystem(const cornersystem_t* cs) {
    // corner ids should be in ascending order, corners should not be rotated
    for (int i = 0; i < 8; i++) {
        if (cs->corners[i].cid != i || cs->corners[i].rotation != 0) {
            return false;
        }
    }
    return true;
}

bool isSolvedEdgeSystem(const edgesystem_t* es) {
    // edge ids should be in ascending order, edges should not be flipped
    for (int i = 0; i < 12; i++) {
        if (es->edges[i].eid != i || es->edges[i].flip != 0) {
            return false;
        }
    }
    return true;
}

