#include <stdlib.h>
#include <stdio.h>

#include <cube_utils.h>
#include <cube.h>
#include <ida_star.h>
#include <corner_lookup.h>
#include <upper_edge_lookup.h>
#include <lower_edge_lookup.h>

#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mut;
pthread_cond_t con;
int answer = -1;

void* theThread(void* a) {
    if (*(int*) a == 2) {
        usleep(1000000);
        pthread_mutex_lock(&mut);
        answer = 1337;
        pthread_cond_signal(&con);
        pthread_mutex_unlock(&mut);
    } else {
        for (int i = 0; i < 20; i++) {
            if (answer != -1) {
                return NULL;
            }
            puts("TEST");
            usleep(100000);
        }
    }
    return NULL;
}

int main2() {
    pthread_t thread[5];
    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&con, NULL);
    for (int i = 0; i < 5; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        pthread_create(&thread[i], NULL, &theThread, a);
    }
    pthread_mutex_lock(&mut);
    pthread_cond_wait(&con, &mut);
    printf("answer: %d\n", answer);
    pthread_mutex_unlock(&mut);
    pthread_exit(NULL);
}
