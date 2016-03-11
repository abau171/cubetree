#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <cube_utils.h>
#include <corner.h>

const cornersystem_t solved_cornersystem = { .corners={
    { .cid=0, .rotation=0 }, { .cid=1, .rotation=0 }, { .cid=2, .rotation=0 },
    { .cid=3, .rotation=0 }, { .cid=4, .rotation=0 }, { .cid=5, .rotation=0 },
    { .cid=6, .rotation=0 }, { .cid=7, .rotation=0 }
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

uint8_t getFaceletCornerSystem(const cornersystem_t* cs, uint8_t face, int i) {
    const corner_t* face_corner = &face_corners[face][i];
    const corner_t* corner = &cs->corners[face_corner->cid];
    uint8_t facelet_rotation = positive_modulo(face_corner->rotation - corner->rotation, 3);
    return corner_facelets[corner->cid][facelet_rotation];
}

bool isSolvedCornerSystem(const cornersystem_t* cs) {
    for (int i = 0; i < 8; i++) {
        if (cs->corners[i].cid != i || cs->corners[i].rotation != 0) {
            return false;
        }
    }
    return true;
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
