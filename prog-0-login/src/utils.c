#include "utils.h"

// Temperature (en degres) des cases chauffees a l'etat initial
int TEMP_CHAUD = 256;



/**
 * Alloue l'espace memoire necessaire a la matrice en parametre.
 * Initialise toutes ses cases a 0 sauf la plaque interne.
 * */
MAT init(int taille, int n) {
    MAT m;
    // Alloue la place pour la matrice.
    if((m = malloc(sizeof(float) * taille * taille)) == NULL) {
        fprintf(stderr,"Allocation impossible dans init.\n");
        exit(EXIT_FAILURE);        
    }
    // Initialisation de la matrice a 0.
    for (int i =0; i<taille; i++) {
    	if ((m[i] = (float *) malloc(sizeof(float) * taille)) == NULL) {
            fprintf(stderr,"Allocation impossible dans init.\n");
            exit(EXIT_FAILURE);  
        }
        for (int j=0; j<taille; j++) {
        	// if ((m[i] = (float *) malloc(sizeof(float) * taille)) == NULL) {
         //    	fprintf(stderr,"Allocation impossible dans init.\n");
         //    	exit(EXIT_FAILURE);  
        	// }
            m[i][j] = 0;
        }
    }
    // Chauffe la plaque centrale
    chauffer_zone_centrale(m, n);

    return m;
}



/**
 * Libere la memoire de la matrice placée en paramètre.
 * */
void free_mat(MAT m, int taille) {
    for (int i =0; i<taille; i++) {
        free((void *) m[i]);
    }
    free(m);
}



/**
 * Chauffe la zone centrale de la matrice placee en parametre, a la temperature TEMP_CHAUD.
 **/
void chauffer_zone_centrale(MAT m, int n) {
    int idMin = (1<<(n-1)) - (1<<(n-4));
    int idMax = (1<<(n-1)) + (1<<(n-4));
    for (int i =  idMin ; i < idMax  ; i++) {
        for (int j =  idMin  ; j <  idMax ; j++) {
            m[i][j] = TEMP_CHAUD;
        }
    }    
}



/**
 * Affiche le nombre d'executions et la taille de la matrice.
 * */
void afficher_options_synthetiques(int taille, int exes) {
    printf("Pour une matrice de taille : %d, nombre d'executions : %d\n",taille,exes);
}



/**
 * Affiche la matrice en parametre.
 * */
void print_matrice(MAT m, int taille) {
    for (int i =0; i<taille; i++) {
        for (int j=0; j<taille; j++) {
            printf("| %.1f ",m[i][j]);
        }
        printf("|\n");
    }
}



/**
 * Affiche le quart superieur gauche de la matrice en parametre.
 * */
void print_quarter_matrice(MAT m, int taille) {
    for (int i =0; i<taille/2; i++) {
        for (int j=0; j<taille/2; j++) {
            printf("| %.1f ",m[i][j]);
        }
        printf("|\n");
    }
}



/**
 * Retourne l'heure actuelle a une precision de 6 decimales.
 * */
double get_process_time() {
    struct rusage usage;
    if( 0 == getrusage(RUSAGE_SELF, &usage) ) {
        return (double)(usage.ru_utime.tv_sec + usage.ru_stime.tv_sec) +
               (double)(usage.ru_utime.tv_usec + usage.ru_stime.tv_usec) / 1.0e6;
    }
    return 0;
}
