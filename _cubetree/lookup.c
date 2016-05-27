#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <cube.h>
#include <lookup.h>

// number of unique corner systems
#define CORNER_LOOKUP_SIZE 88179840

// number of unique upper or lower edge systems
#define EDGE_LOOKUP_SIZE 42577920

// factorial cache
static int fact[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800};

// n choose r
static int ncr(int n, int r) {
    return fact[n] / (fact[r] * fact[n - r]);
}

/* mmaped lookup tables */
static uint8_t corner_lookup[CORNER_LOOKUP_SIZE];
static uint8_t upper_edge_lookup[EDGE_LOOKUP_SIZE];
static uint8_t lower_edge_lookup[EDGE_LOOKUP_SIZE];

/* generator queue node definitions*/

typedef struct corner_queuenode_t {
    cornersystem_t cs;
    uint8_t distance;
    struct corner_queuenode_t* next;
} corner_queuenode_t;

typedef struct edge_queuenode_t {
    edgesystem_t es;
    uint8_t distance;
    struct edge_queuenode_t* next;
} edge_queuenode_t;


/* lookup functions */

uint8_t lookupCornerDistance(int cornersystem_encoding) {
    return corner_lookup[cornersystem_encoding];
}

uint8_t lookupUpperEdgeDistance(int edgesystem_encoding) {
    return upper_edge_lookup[edgesystem_encoding];
}

uint8_t lookupLowerEdgeDistance(int edgesystem_encoding) {
    return lower_edge_lookup[edgesystem_encoding];
}

/* encoding functions */

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

int encodeUpperEdgeSystem(const edgesystem_t* es) {
    int key = 0;
    int comb[6];
    int combI = 0;
    for (int i = 0; i < 12; i++) {
        if (es->edges[i].eid < 6) {
            comb[combI] = i + 1;
            combI++;
        }
    }
    int j = 0;
    for (int i = 0; i < 6; i++) {
        for (j++; j < comb[i]; j++) {
            key += ncr(12 - j, 6 - i - 1);
        }
    }
    key *= 720;
    int perm[6];
    int permI = 0;
    for (int i = 0; i < 12; i++) {
        if (es->edges[i].eid < 6) {
            perm[permI] = es->edges[i].eid;
            permI++;
        }
    }
    for (int i = 0; i < 5; i++) {
        int numLess = 0;
        for (int j = i + 1; j < 6; j++) {
            if (perm[i] > perm[j]) {
                numLess++;
            }
        }
        key += numLess * fact[5 - i];
    }
    for (int i = 0; i < 12; i++) {
        if (es->edges[i].eid < 6) {
            key *= 2;
            key += es->edges[i].flip;
        }
    }
    return key;
}

int encodeLowerEdgeSystem(const edgesystem_t* es) {
	int key = 0;
	int j = 0;
	int k = 0;
	for (int i = 0; i < 12; i++) {
		if (es->edges[i].eid >= 6) {
			for (k++; k < i + 1; k++) {
				key += ncr(12 - k, 6 - j - 1);
			}
			j++;
		}
	}
	key *= 720;
	int perm[6];
	int permI = 0;
	for (int i = 0; i < 12; i++) {
		if (es->edges[i].eid >= 6) {
			perm[permI] = es->edges[i].eid - 6;
			permI++;
		}
	}
	for (int i = 0; i < 5; i++) {
		int numLess = 0;
		for (int j = i + 1; j < 6; j++) {
			if (perm[i] > perm[j]) {
				numLess++;
			}
		}
		key += numLess * fact[5 - i];
	}
	for (int i = 0; i < 12; i++) {
        if (es->edges[i].eid >= 6) {
    		key *= 2;
    		key += es->edges[i].flip;
        }
	}
	return key;
}


/* generator functions */

static bool (*cancel_checker)(int, int);

void set_lookup_cancel_checker(bool (*checker)(int, int)) {
    cancel_checker = checker;
}

void genCornerLookup() {
    for (int i = 0; i < CORNER_LOOKUP_SIZE; i++) {
        corner_lookup[i] = 255;
    }
    corner_queuenode_t* queue = malloc(sizeof(corner_queuenode_t));
    queue->cs = solved_cornersystem;
    queue->distance = 0;
    queue->next = NULL;
    corner_queuenode_t* queue_end = queue;
    corner_lookup[encodeCornerSystem(&(queue->cs))] = 0;
    int count = 0;
    while (queue != NULL) {
        count += 1;
        if (count % 1000000 == 0) {
            cancel_checker(count, CORNER_LOOKUP_SIZE);
        }
        for (uint8_t face = 0; face < 6; face++) {
            for (int turn_type = 1; turn_type < 4; turn_type++) {
                cornersystem_t new_cs;
                turnCornerSystem(&new_cs, &queue->cs, face, turn_type);
                int code = encodeCornerSystem(&new_cs);
                if (corner_lookup[code] == 255) {
                    corner_lookup[code] = queue->distance + 1;
                    corner_queuenode_t* new_end = malloc(sizeof(corner_queuenode_t));
                    new_end->cs = new_cs;
                    new_end->distance = queue->distance + 1;
                    new_end->next = NULL;
                    queue_end->next = new_end;
                    queue_end = new_end;
                }
            }
        }
        corner_queuenode_t* nextQueue = queue->next;
        free(queue);
        queue = nextQueue;
    }
}

void genUpperEdgeLookup() {
    for (int i = 0; i < EDGE_LOOKUP_SIZE; i++) {
        upper_edge_lookup[i] = 255;
    }
    edge_queuenode_t* queue = malloc(sizeof(edge_queuenode_t));
    queue->es = solved_edgesystem;
    queue->distance = 0;
    queue->next = NULL;
    edge_queuenode_t* queue_end = queue;
    upper_edge_lookup[encodeUpperEdgeSystem(&(queue->es))] = 0;
    int count = 0;
    while (queue != NULL) {
        count += 1;
        if (count % 1000000 == 0) {
            cancel_checker(count, CORNER_LOOKUP_SIZE);
        }
        for (uint8_t face = 0; face < 6; face++) {
            for (int turn_type = 1; turn_type < 4; turn_type++) {
                edgesystem_t new_es;
                turnEdgeSystem(&new_es, &queue->es, face, turn_type);
                int code = encodeUpperEdgeSystem(&new_es);
                if (upper_edge_lookup[code] == 255) {
                    upper_edge_lookup[code] = queue->distance + 1;
                    edge_queuenode_t* new_end = malloc(sizeof(edge_queuenode_t));
                    new_end->es = new_es;
                    new_end->distance = queue->distance + 1;
                    new_end->next = NULL;
                    queue_end->next = new_end;
                    queue_end = new_end;
                }
            }
        }
        edge_queuenode_t* next_queue = queue->next;
        free(queue);
        queue = next_queue;
    }
}

void genLowerEdgeLookup() {
    for (int i = 0; i < EDGE_LOOKUP_SIZE; i++) {
        lower_edge_lookup[i] = 255;
    }
    edge_queuenode_t* queue = malloc(sizeof(edge_queuenode_t));
    queue->es = solved_edgesystem;
    queue->distance = 0;
    queue->next = NULL;
    edge_queuenode_t* queue_end = queue;
    lower_edge_lookup[encodeLowerEdgeSystem(&(queue->es))] = 0;
    int count = 0;
    while (queue != NULL) {
        count += 1;
        if (count % 1000000 == 0) {
            cancel_checker(count, CORNER_LOOKUP_SIZE);
        }
        for (uint8_t face = 0; face < 6; face++) {
            for (int turn_type = 1; turn_type < 4; turn_type++) {
                edgesystem_t new_es;
                turnEdgeSystem(&new_es, &queue->es, face, turn_type);
                int code = encodeLowerEdgeSystem(&new_es);
                if (lower_edge_lookup[code] == 255) {
                    lower_edge_lookup[code] = queue->distance + 1;
                    edge_queuenode_t* new_end = malloc(sizeof(edge_queuenode_t));
                    new_end->es = new_es;
                    new_end->distance = queue->distance + 1;
                    new_end->next = NULL;
                    queue_end->next = new_end;
                    queue_end = new_end;
                }
            }
        }
        edge_queuenode_t* next_queue = queue->next;
        free(queue);
        queue = next_queue;
    }
}


/* cache file management functions */

bool loadCornerLookup() {
    FILE* file = fopen("cache/corner.cache", "rb");
    if (file == NULL)
        return false;
    size_t bytes_read = fread(corner_lookup, sizeof(uint8_t), CORNER_LOOKUP_SIZE, file);
    fclose(file);
    if (bytes_read < CORNER_LOOKUP_SIZE)
        return false;
    return true;
}

bool loadUpperEdgeLookup() {
    FILE* file = fopen("cache/upper_edge.cache", "rb");
    if (file == NULL)
        return false;
    size_t bytes_read = fread(upper_edge_lookup, sizeof(uint8_t), EDGE_LOOKUP_SIZE, file);
    fclose(file);
    if (bytes_read < EDGE_LOOKUP_SIZE)
        return false;
    return true;
}

bool loadLowerEdgeLookup() {
    FILE* file = fopen("cache/lower_edge.cache", "rb");
    if (file == NULL)
        return false;
    size_t bytes_read = fread(lower_edge_lookup, sizeof(uint8_t), EDGE_LOOKUP_SIZE, file);
    fclose(file);
    if (bytes_read < EDGE_LOOKUP_SIZE)
        return false;
    return true;
}

bool saveCornerLookup() {
    FILE* file = fopen("cache/corner.cache", "wb");
    if (file == NULL)
        return false;
    size_t bytes_written = fwrite(corner_lookup, sizeof(uint8_t), CORNER_LOOKUP_SIZE, file);
    fclose(file);
    if (bytes_written < CORNER_LOOKUP_SIZE)
        return false;
    return true;
}

bool saveUpperEdgeLookup() {
    FILE* file = fopen("cache/upper_edge.cache", "wb");
    if (file == NULL)
        return false;
    size_t bytes_written = fwrite(upper_edge_lookup, sizeof(uint8_t), EDGE_LOOKUP_SIZE, file);
    fclose(file);
    if (bytes_written < EDGE_LOOKUP_SIZE)
        return false;
    return true;
}

bool saveLowerEdgeLookup() {
    FILE* file = fopen("cache/lower_edge.cache", "wb");
    if (file == NULL)
        return false;
    size_t bytes_written = fwrite(lower_edge_lookup, sizeof(uint8_t), EDGE_LOOKUP_SIZE, file);
    fclose(file);
    if (bytes_written < EDGE_LOOKUP_SIZE)
        return false;
    return true;
}

