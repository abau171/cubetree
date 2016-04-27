#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

#include <cube.h>
#include <ida_star.h>

#define NUM_THREADS 8

typedef struct idastarjob_t {
	uint8_t face;
	int turn_type;
	cube_t start_cube;
	struct idastarjob_t* next_job;
} idastarjob_t;

typedef struct {
	pthread_t threads[8];
	pthread_mutex_t mutex;
	pthread_cond_t jobs_available_cond;
	pthread_cond_t thread_state_changed_cond;
	int num_waiting;
	int depth;
	idastarjob_t* job_queue;
	bool solution_found;
	movenode_t* solution;
} idastarpool_t;

void* idaStarThread(void* arg) {
	idastarpool_t* pool = arg;
	pthread_mutex_lock(&pool->mutex);
	while (true) {
		while (pool->job_queue == NULL && !pool->solution_found) {
			pool->num_waiting++;
			pthread_cond_broadcast(&pool->thread_state_changed_cond);
			pthread_cond_wait(&pool->jobs_available_cond, &pool->mutex);
			pool->num_waiting--;
		}
		if (pool->solution_found) break;
		idastarjob_t* cur_job = pool->job_queue;
		pool->job_queue = cur_job->next_job;
		printf("starting job: Face %d, Turn %d\n", cur_job->face, cur_job->turn_type);
		pthread_mutex_unlock(&pool->mutex);

		// process state
		movenode_t* solution = searchDepth(&cur_job->start_cube, pool->depth - 1, cur_job->face, &pool->solution_found);
		if (solution != NULL) {
		    pool->solution_found = true;
		    pool->solution = prependMoveNode(solution, cur_job->face, cur_job->turn_type);
		}

		pthread_mutex_lock(&pool->mutex);
	}
	pthread_cond_broadcast(&pool->thread_state_changed_cond);
	pthread_mutex_unlock(&pool->mutex);
	return NULL;
}

movenode_t* multithreadedIdaStar(const cube_t* cube) {

	// init pool
	idastarpool_t pool;
	pthread_mutex_init(&pool.mutex, NULL);
	pthread_cond_init(&pool.jobs_available_cond, NULL);
	pthread_cond_init(&pool.thread_state_changed_cond, NULL);
	pool.num_waiting = 0;
	pool.depth = 1;
	pool.job_queue = NULL;
	pool.solution_found = false;
	pool.solution = NULL;

	pthread_mutex_lock(&pool.mutex);

	// start up threads
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_create(&pool.threads[i], NULL, &idaStarThread, &pool);
	}
	while (pool.num_waiting < NUM_THREADS) {
		pthread_cond_wait(&pool.thread_state_changed_cond, &pool.mutex);
	}

	// start job queue/search loop
	while (!pool.solution_found) {
		pool.depth++;
		printf("DEPTH %d\n", pool.depth);
		for (uint8_t face = 5; face != 255; face--) {
			for (int turn_type = 3; turn_type >= 1; turn_type--) {
				idastarjob_t* new_job = malloc(sizeof(idastarjob_t));
				new_job->face = face;
				new_job->turn_type = turn_type;
				turnCube(&new_job->start_cube, cube, face, turn_type);
				new_job->next_job = pool.job_queue;
				pool.job_queue = new_job;
			}
		}
		pthread_cond_broadcast(&pool.jobs_available_cond);
		while ((pool.num_waiting < NUM_THREADS || pool.job_queue != NULL) && !pool.solution_found) {
			pthread_cond_wait(&pool.thread_state_changed_cond, &pool.mutex);
		}
	}
	pthread_cond_broadcast(&pool.jobs_available_cond);
	pthread_mutex_unlock(&pool.mutex);
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(pool.threads[i], NULL);
	}




	// int depth = 0;
	// bool cancel_flag = false;
	// while (true) {
	// 	depth++;
	// 	printf("DEPTH %d\n", depth);
	// 	movenode_t* result = searchDepth(cube, depth, 6, &cancel_flag);
	// 	if (result != NULL) {
	// 		return result;
	// 	}
	// }
	return pool.solution;
}
