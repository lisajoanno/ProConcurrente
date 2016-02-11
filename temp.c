#include <stdio.h>
#include <stdlib.h> // pour malloc
#include <unistd.h> // pour getopt
#include <getopt.h>

#define TEMP_FROID 0;
#define TEMP_CHAUD 256;

int TAILLE_MATRICE; // taille de la matrice entrée par l'utilisateur
int MES_AFF_CPU; // 1 : mesure et affichage du temps d'execution (consommation du CPU); 0 : non
int MES_AFF_REPUSER; // 1 : mesure et affichage du temps d'execution (temps de réponse utilisateur); 0 : non
int AFF; // 1 : affichage de la température initiale et de la température finale  pour les indices  correspondant au quart supérieur gauche du tableau; 0 : non
int NB_EXE; // nombre d'itérations à éxecuter
int ETAPES; // étapes du programme à exécuter
int NB_THREADS; // nombre de threads a créer

typedef struct {
  int taille;
  float* data;
} matrice;

typedef float **MAT;

/**
Initialisations par défaut des options du système (dans le cas où les options ne sont pas précisées par l'utilisateur).
**/
void init_options_par_defaut() {
	TAILLE_MATRICE = 024; 
	MES_AFF_CPU = 1; 
	MES_AFF_REPUSER = 0;
	AFF = 0; 
	NB_EXE = 10000; 
	ETAPES = 012345;
	NB_THREADS = 13;
}

matrice* init_matrice() {
	matrice* mat = malloc(sizeof(matrice));
	mat->taille = TAILLE_MATRICE;
	mat->data = malloc(mat->taille * mat->taille * sizeof(float));
	int i;
	for (i = 0; i < mat->taille*mat->taille; i++) {
    	mat->data[i] = 0.0;
	}
	
	return mat;
}

MAT init() {
	MAT mat = malloc(sizeof(MAT));
	return mat;
}

void afficher_options() {
	printf("Options : \n");
	printf("TAILLE_MATRICE = %d\n",TAILLE_MATRICE); 
	printf("MES_AFF_CPU = %d\n", MES_AFF_CPU); 
	printf("MES_AFF_REPUSER = %d\n", MES_AFF_REPUSER); 
	printf("AFF = %d\n", AFF); 
	printf("NB_EXE = %d\n", NB_EXE); 
	printf("ETAPES = %d\n", ETAPES); 
	printf("NB_THREADS = %d\n",NB_THREADS); 
}

/**
Sauvegarde des options précisées par l'utilisateur dans les variables prévues à cet effet.
**/
void capter_options(int argc, char *argv[]) {

	MES_AFF_CPU = 0; 
	MES_AFF_REPUSER = 0;
	AFF = 0; 
	NB_EXE = 10000;



	int option = 0;
    //Specifying the expected options
    //The two options l and b expect numbers as argument
    while ((option = getopt(argc, argv,"s:mMai:e:t:")) != -1) {
        switch (option) {
            case 's' : 
             	if (atoi(optarg) <= 9 && atoi(optarg) >= 0) {
             		TAILLE_MATRICE = atoi(optarg);
             	} else {
             		TAILLE_MATRICE = atoi(optarg);
             	}
                break;
            case 'm' : 
             	MES_AFF_CPU = 1; 
                break;
            case 'M' : 
             	MES_AFF_REPUSER = 1;
                break;
            case 'a' : 
            	AFF = 1;
                break;
            case 'i' : 
            	NB_EXE = atoi(optarg);
                break;
            case 'e' : 
            	if (atoi(optarg) <= 6 && atoi(optarg) >= 0) {
             		ETAPES = atoi(optarg);
             	} else {
             		ETAPES = atoi(optarg);
             	}
                break;
            case 't' : 
            	if (atoi(optarg) <= 6 && atoi(optarg) >= 0) {
             		NB_THREADS = atoi(optarg);
             	} else {
             		NB_THREADS = atoi(optarg);
             	}
                break;
            default: afficher_options();
                exit(EXIT_FAILURE);
        }
    }
}


int main(int argc, char *argv[]) {
	printf("Programme de test\n");

	if (argc == 1) {
		printf("Il n'y a pas eu d'argument.\n");
		init_options_par_defaut();
	} else {
		printf("Il y a eu des arguments.\n");
		capter_options(argc,argv);
	}
	afficher_options();

	// matrice* mat = init_matrice();
	MAT mat = init();

	

	return 0;
}