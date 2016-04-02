#ifndef _THREAD_BARRIERE_H
#define _THREAD_BARRIERE_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include "utils.h"
#include "diffusion_chaleur.h"

// Declaration des barrieres posix
pthread_barrier_t barrier;
pthread_barrier_t barrierX;
pthread_barrier_t barrierY;

typedef struct ThreadParam {
	MAT mat_courante;
	MAT mat_prec;
    int x_init;
    int y_init;
    int x_fin;
    int y_fin;
    int n;
    int taille;
} ThreadParam;

void *thread(void *attr);
void init_threads(MAT mat_courante, MAT mat_prec, int n, int taille, int nb_thread, int t);

#endif