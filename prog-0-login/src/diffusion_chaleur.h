#ifndef _DIFFUSION_CHALEUR_H
#define _DIFFUSION_CHALEUR_H


/******************* PARAMETRES DU SYSTEME ****************************/

// Temperatures (en degres) des cases non chauffees a l'etat initial
int TEMP_FROID = 0;
// Temperature (en degres) des cases chauffees a l'etait initial
int TEMP_CHAUD = 256;
// Taille de la matrice courante entree par l'utilisateur (taille = (n+4)**2)
int n;
// Taille de la matrice courante
int TAILLE_MATRICE; 
// 1 : mesure et affichage du temps d'execution (consommation du CPU)
// 0 : non
int MES_AFF_CPU; 
// 1 : mesure et affichage du temps d'execution (temps de reponse utilisateur)
// 0 : non
int MES_AFF_REPUSER; 
// 1 : affichage de la temperature initiale et de la temperature finale pour les indices correspondant au quart superieur gauche du tableau
// 0 : non
int AFF; 
// Nombre d'iterations a executer
int NB_EXE; 
// Etape du programme a executer
int ETAPES; 
// Nombre de threads a creer
int NB_THREADS; 
// Nombre de cases de la zone interne
int TAILLE_ZONE_INT;

// Constantes utilisee dans la formule de Taylor, fixee ici a 6
float H = 6;

// Chaines de caracteres contenant les options de l'utilisateur.
// etapes, par exemple : "012345"
char* etapes;
// threads, par exemple "13"
char* threads;
// tailles, par exemple "024"
char* tailles;


/**************** DECLARATION & INITIALISATION DE LA MATRICE ****************/

// La matrice est representee par un float**
typedef float **MAT;

// On definit deux matrices : celle de l'etape n (mat_courante) et celle de l'etape n-1 (mat_prec)
MAT mat_prec, mat_courante;


MAT init();
int powi(int number, int exponent);
void free_mat();
void print_matrice(MAT m);
void print_quarter_matrice(MAT m);
double get_process_time();
void afficher_options_synthetiques();
void capter_options(int argc, char *argv[]);
void diffuser_chaleur_x();
void diffuser_chaleur_y();
void chauffer_zone_centrale();
void calculer_temps_cpu();
void calculer_temps_user();
void lancer_algo_affichage();
void lancer_selon_options();

#endif
