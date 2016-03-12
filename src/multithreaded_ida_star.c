#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

#include <cube.h>
#include <ida_star.h>

#define NUM_THREADS 8

typedef struct cubejob_t {
	cube_t cube;
	struct cubejob_t* next;
} cubejob_t;

typedef struct {
	pthread_t threads[NUM_THREADS];
	pthread_mutex_t pool_mutex;
	pthread_cond_t start_job_cond;
	pthread_cond_t finish_job_cond;
	cubejob_t* job_queue;
	int num_waiting_threads;
	int num_active_threads;
	int depth;
	movenode_t* solution;
} cubethreadpool_t;

void* cubeThread(void* voidPool) {
	cubethreadpool_t* pool = (cubethreadpool_t*) voidPool;
	pthread_mutex_lock(&pool->pool_mutex);
	while (pool->solution == NULL) {
		if (pool->job_queue == NULL) {
			pool->num_waiting_threads++;
			pthread_cond_broadcast(&pool->finish_job_cond);
			pthread_cond_wait(&pool->start_job_cond, &pool->pool_mutex);
			pool->num_waiting_threads--;
		} else {
			cubejob_t* job = pool->job_queue;
			pool->job_queue = job->next;
			pool->num_active_threads++;
			pthread_mutex_unlock(&pool->pool_mutex);
			movenode_t* solution = searchDepth(&job->cube, pool->depth - 1, 6);
			pthread_mutex_lock(&pool->pool_mutex);
			if (solution != NULL) {
				pool->solution = solution;
				puts("FOUND SOLUTION");
			}
			pool->num_active_threads--;
		}
	}
	pthread_mutex_unlock(&pool->pool_mutex);
	puts("THREAD DEAD");
	return NULL;
}

cubethreadpool_t* newCubeThreadPool() {
	cubethreadpool_t* pool = malloc(sizeof(cubethreadpool_t));
	pthread_mutex_init(&pool->pool_mutex, NULL);
	pthread_cond_init(&pool->start_job_cond, NULL);
	pthread_cond_init(&pool->finish_job_cond, NULL);
	pool->job_queue = NULL;
	pool->num_waiting_threads = 0;
	pool->num_active_threads = 0;
	pool->solution = NULL;
	pthread_mutex_lock(&pool->pool_mutex);
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_create(&pool->threads[i], NULL, &cubeThread, (void*) pool);
	}
	while (pool->num_waiting_threads < NUM_THREADS) {
		pthread_cond_wait(&pool->finish_job_cond, &pool->pool_mutex);
	}
	pthread_mutex_unlock(&pool->pool_mutex);
	return pool;
}

movenode_t* multithreadedIdaStar(const cube_t* cube) {
	cubethreadpool_t* pool = newCubeThreadPool();
	pthread_mutex_lock(&pool->pool_mutex);
	pool->depth = 3;
	while (true) {
		printf("DEPTH %d\n", pool->depth);
		for (uint8_t face = 0; face < 6; face++) {
			for (int turn_type = 1; turn_type < 4; turn_type++) {
				cubejob_t* job = malloc(sizeof(cubejob_t));
				turnCube(&job->cube, cube, face, turn_type);
				job->next = pool->job_queue;
				pool->job_queue = job;
			}
		}
		pthread_cond_broadcast(&pool->start_job_cond);

		while (pool->job_queue != NULL || pool->num_active_threads > 0) {
			pthread_cond_wait(&pool->finish_job_cond, &pool->pool_mutex);
		}

		if (pool->solution != NULL) {
			// puts("SOLUTION AVAILABLE");
		} else {
			// puts("NO SOLUTION AVAILABLE");
		}
		pool->depth++;
	}
	pthread_mutex_unlock(&pool->pool_mutex);
}
