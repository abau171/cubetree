#include <stdint.h>
#include <stdio.h>

#include <cube_utils.h>
#include <edge.h>

const edgesystem_t solved_edgesystem = { .edges={
    { .eid=0, .flip=0 }, { .eid=1, .flip=0 }, { .eid=2, .flip=0 },
    { .eid=3, .flip=0 }, { .eid=4, .flip=0 }, { .eid=5, .flip=0 },
    { .eid=6, .flip=0 }, { .eid=7, .flip=0 }, { .eid=8, .flip=0 },
    { .eid=9, .flip=0 }, { .eid=10, .flip=0 }, { .eid=11, .flip=0 }
}};

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

uint8_t getFaceletEdgeSystem(const edgesystem_t* es, uint8_t face, int i) {
    const edge_t* face_edge = &face_edges[face][i];
    const edge_t* edge = &es->edges[face_edge->eid];
    uint8_t facelet_flip = positive_modulo(face_edge->flip - edge->flip, 2);
    return edge_facelets[edge->eid][facelet_flip];
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
