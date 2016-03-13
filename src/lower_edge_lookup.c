#include <stdlib.h>
#include <stdio.h>

#include <edge.h>
#include <lower_edge_lookup.h>

static int fact[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800};

static int ncr(int n, int r) {
    return fact[n] / (fact[r] * fact[n - r]);
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

static uint8_t lower_edge_lookup[42577920];

typedef struct queuenode_t {
    edgesystem_t es;
    uint8_t distance;
    struct queuenode_t* next;
} queuenode_t;

void genLowerEdgeLookup() {
    for (int i = 0; i < 42577920; i++) {
        lower_edge_lookup[i] = 255;
    }
    queuenode_t* queue = malloc(sizeof(queuenode_t));
    queue->es = solved_edgesystem;
    queue->distance = 0;
    queue->next = NULL;
    queuenode_t* queue_end = queue;
    lower_edge_lookup[encodeLowerEdgeSystem(&(queue->es))] = 0;
    int count = 0;
    while (queue != NULL) {
        count += 1;
        if (count % 1000000 == 0) {
            printf("%3.2f%% complete (%d/42577920)\n", 100.0 * count / 42577920.0, count);
        }
        for (uint8_t face = 0; face < 6; face++) {
            for (int turn_type = 1; turn_type < 4; turn_type++) {
                edgesystem_t new_es;
                turnEdgeSystem(&new_es, &queue->es, face, turn_type);
                int code = encodeLowerEdgeSystem(&new_es);
                if (lower_edge_lookup[code] == 255) {
                    lower_edge_lookup[code] = queue->distance + 1;
                    queuenode_t* new_end = malloc(sizeof(queuenode_t));
                    new_end->es = new_es;
                    new_end->distance = queue->distance + 1;
                    new_end->next = NULL;
                    queue_end->next = new_end;
                    queue_end = new_end;
                }
            }
        }
        queuenode_t* next_queue = queue->next;
        free(queue);
        queue = next_queue;
    }
}

uint8_t lookupLowerEdgeDistance(int edgesystem_encoding) {
    return lower_edge_lookup[edgesystem_encoding];
}

void saveLowerEdgeLookupCache() {
    FILE* file = fopen("cache/lower_edge.cache", "wb");
    fwrite(lower_edge_lookup, sizeof(char), 42577920, file);
    fclose(file);
}

void loadLowerEdgeLookupCache() {
    FILE* file = fopen("cache/lower_edge.cache", "rb");
    fread(lower_edge_lookup, sizeof(char), 42577920, file);
    fclose(file);
}
