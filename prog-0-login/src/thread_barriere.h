#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

// Declaration des barrieres posix
pthread_barrier_t barrier;
pthread_barrier_t barrierX;
pthread_barrier_t barrierY;

struct ThreadParam {
    int x_init;
    int y_init;
    int x_fin;
    int y_fin;
};

void *thread();
void init_threads();
void init_thread();
int main();
