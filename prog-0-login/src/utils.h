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



// Initialise la matrice en parametre
MAT init(int,int);
// Libere la matrice en parametre
void free_mat(MAT,int);
// Chauffe la zone centrale de la matrice placee en parametre
void chauffer_zone_centrale(MAT,int);

// Affiche la taille de la matrice courante et le nombre d'execution
void afficher_options_synthetiques(int taille_courante, int nb_exe_courant);
// Affiche toute la matrice en parametre
void print_matrice(MAT m, int);
// Affiche le quart superieur gauche de la matrice en parametre
void print_quarter_matrice(MAT m, int);

// Renvoie l'heure actuelle
double get_process_time();

#endif