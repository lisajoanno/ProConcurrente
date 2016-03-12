#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdlib.h> // pour malloc
#include <unistd.h> // pour getopt
#include <getopt.h> // pour getopt
#include <time.h> // pour time et la constante CLOCKS_PER_SEC
#include <sys/time.h> // pour l'heure
#include <sys/resource.h> // pour l'heure


// La matrice est representee par un float**
typedef float **MAT;

MAT init(int,int n);
void free_mat(MAT,int);

void chauffer_zone_centrale(MAT,int);

int powi(int number, int exponent);
void afficher_options_synthetiques(int, int);
void print_matrice(MAT m, int);
void print_quarter_matrice(MAT m, int);

double get_process_time();

#endif