#include <stdlib.h>
#include <stdio.h>

#include <corner.h>

static int fact[] = {1, 1, 2, 6, 24, 120, 720, 5040};

int encodeCornerSystem(const cornersystem_t* cs) {
    int key = 0;
    for (int i = 0; i < 7; i++) {
        int numLess = 0;
        for (int j = i + 1; j < 8; j++) {
            if (cs->corners[i].cid > cs->corners[j].cid) {
                numLess++;
            }
        }
        key += numLess * fact[7 - i];
    }
    for (int i = 0; i < 7; i++) {
        key *= 3;
        key += cs->corners[i].rotation;
    }
    return key;
}

static uint8_t corner_lookup[88179840];

typedef struct queuenode_t {
    cornersystem_t cs;
    uint8_t distance;
    struct queuenode_t* next;
} queuenode_t;

void genCornerLookup() {
    for (int i = 0; i < 88179840; i++) {
        corner_lookup[i] = 255;
    }
    queuenode_t* queue = malloc(sizeof(queuenode_t));
    queue->cs = solved_cornersystem;
    queue->distance = 0;
    queue->next = NULL;
    queuenode_t* queue_end = queue;
    corner_lookup[encodeCornerSystem(&(queue->cs))] = 0;
    int count = 0;
    while (queue != NULL) {
        count += 1;
        if (count % 1000000 == 0) {
            printf("%3.2f%% complete (%d/88179840)\n", 100.0 * count / 88179840.0, count);
        }
        for (uint8_t face = 0; face < 6; face++) {
            for (int turn_type = 1; turn_type < 4; turn_type++) {
                cornersystem_t new_cs;
                turnCornerSystem(&new_cs, &queue->cs, face, turn_type);
                int code = encodeCornerSystem(&new_cs);
                if (corner_lookup[code] == 255) {
                    corner_lookup[code] = queue->distance + 1;
                    queuenode_t* new_end = malloc(sizeof(queuenode_t));
                    new_end->cs = new_cs;
                    new_end->distance = queue->distance + 1;
                    new_end->next = NULL;
                    queue_end->next = new_end;
                    queue_end = new_end;
                }
            }
        }
        queuenode_t* nextQueue = queue->next;
        free(queue);
        queue = nextQueue;
    }
}

uint8_t lookupCornerDistance(int cornersystem_encoding) {
    return corner_lookup[cornersystem_encoding];
}

void saveCornerLookupCache() {
    FILE* file = fopen("cache/corner.cache", "wb");
    fwrite(corner_lookup, sizeof(char), 88179840, file);
    fclose(file);
}

void loadCornerLookupCache() {
    FILE* file = fopen("cache/corner.cache", "rb");
    fread(corner_lookup, sizeof(char), 88179840, file);
    fclose(file);
}
