#include <stdio.h>
#include <string.h>
#include <stdlib.h> // pour malloc
#include <unistd.h> // pour getopt
#include <getopt.h> // pour getopt
#include <time.h> // pour time et la constante CLOCKS_PER_SEC
#include <sys/time.h> // pour l'heure
#include <sys/resource.h> // pour l'heure

/**
 * 
 * Projet de programmation concurrente : diffusion de la chaleur
 * SI4 2015/2016
 * 
 * Auteurs : Arnaud Garnier & Lisa Joanno
 * 
 * Le programme suivant permet de visualiser le phenomene de diffusion de la chaleur sur une
 * plaque a deux dimensions.
 * 
 * **/


/******************* PARAMETRES DU SYSTEME ****************************/

// Temperatures (en degres) des cases non chauffees a l'etat initial
int TEMP_FROID = 0;
// Temperature (en degres) des cases chauffees a l'etait initial
int TEMP_CHAUD = 256;
// Taille de la matrice courante entree par l'utilisateur (taille = n+4)
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



/*********** DECLARATION & INITIALISATION DE LA MATRICE ****************/

// La matrice est representee par un float**
typedef float **MAT;

// On definit deux matrices : celle de l'etape n et celle de l'etape n-1
MAT mat, current;

/**
 * Reimplementation de la fonction pow a cause des problemes causes par celle de la librairie math.h.
 * */
int powi(int number, int exponent)
{
	int i, product = 1;
	for (i = 0; i < exponent; i++)
		product *= number;
	return product;
}

/**
 * Alloue l'espace memoire necessaire a la matrice.
 * Initialise toutes les cases a 0 sauf la plaque interne.
 * */
void init() {
    // Alloue la place pour la matrice
	mat = malloc(sizeof(float) * TAILLE_MATRICE * TAILLE_MATRICE);
    // Initialisation de la matrice a 0 partout
    for (int i =0; i<TAILLE_MATRICE; i++) {
        for (int j=0; j<TAILLE_MATRICE; j++) {
            mat[i] = (float *) malloc(sizeof(float) * TAILLE_MATRICE);
            mat[i][j] = 0;
        }
    }
    
    // Initialisation de la zone chauffee initialement
    int idMin =  powi(2,n-1) - powi(2,n-4);
    int idMax = powi(2,n-1) + powi(2,n-4);
    TAILLE_ZONE_INT = idMax - idMin;
    for (int i =  idMin ; i < idMax  ; i++) {
        for (int j =  idMin  ; j <  idMax ; j++) {
            mat[i][j] = TEMP_CHAUD;
        }
    }
    
    // Copie de mat dans current
    current = malloc(sizeof(float) * TAILLE_MATRICE * TAILLE_MATRICE);
    for (int i =0; i<TAILLE_MATRICE; i++) {
        for (int j=0; j<TAILLE_MATRICE; j++) {
            current[i] = (float *) malloc(sizeof(float) * TAILLE_MATRICE);
            current [i][j] = 0;
        }
    }
    
    for (int i = 0; i < TAILLE_MATRICE; i++) {
        for(int j = 0; j < TAILLE_MATRICE; j++) {
            current[i][j] = mat[i][j];
        }
    }
}

/**
 * Libere la memoire des deux matrices initialisees.
 * */
void free_mat() {
    for (int i =0; i<TAILLE_MATRICE; i++) {
        free((void *) mat[i]);
    }
    free(mat);
}

/**
 * Affiche la matrice en parametre.
 * */
void print_matrice(MAT m) {
    for (int i =0; i<TAILLE_MATRICE; i++) {
        for (int j=0; j<TAILLE_MATRICE; j++) {
            printf("| %.1f ",m[i][j]);
        }
        printf("|\n");
    }
}

/**
 * Affiche le quart superieur gauche de la matrice en parametre.
 * */
void print_quarter_matrice(MAT m) {
    for (int i =0; i<TAILLE_MATRICE/2; i++) {
        for (int j=0; j<TAILLE_MATRICE/2; j++) {
            printf("| %.1f ",m[i][j]);
        }
        printf("|\n");
    }
}




/*********************** RECUPERATION DES OPTIONS *********************/

/**
 * Affiche les options courantes du programme.
 * */
void afficher_options() {
	printf("\nOptions : \n");
	printf("TAILLE_MATRICE = %d\n",TAILLE_MATRICE); 
	printf("MES_AFF_CPU = %d\n", MES_AFF_CPU); 
	printf("MES_AFF_REPUSER = %d\n", MES_AFF_REPUSER); 
	printf("AFF = %d\n", AFF); 
	printf("NB_EXE = %d\n", NB_EXE); 
	printf("ETAPES = %d\n", ETAPES); 
	printf("NB_THREADS = %d\n",NB_THREADS); 
    printf("\n");
}

/**
 * Retourne la taille et le nombre d'executions sous forme de chaine de caracteres.
 * */
void afficher_options_synthetiques() {
    printf("Pour une matrice de taille : %d, nombre d'executions : %d\n",TAILLE_MATRICE,NB_EXE);
}


/**
 * Sauvegarde des options precisees par l'utilisateur dans les variables prevues a cet effet.
 * */
void capter_options(int argc, char *argv[]) {
    // A 0 par defaut, elles seront mises a 1 si presentes dans les options.
	MES_AFF_CPU = 0;
	MES_AFF_REPUSER = 0;
	AFF = 0; 

	int option = 0;
    // Specification des options acceptees : -s, -r, -t, -i, -m, -M et -a
    while ((option = getopt(argc, argv,"i:e:s:t:mMa")) != -1) {
        switch (option) {
            case 'i' : 
            	NB_EXE = atoi(optarg);
                break;
            case 'e' :
                etapes = optarg;
                break;
            case 's' : 
                tailles = optarg;
                break;
            case 't' : 
                threads = optarg;
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
            default: afficher_options();
                exit(EXIT_FAILURE);
        }
    }

    // Suppression de toute trace d'execution si -m ou -M
    if(MES_AFF_REPUSER || MES_AFF_CPU) {
        AFF = 0;
    }
}




/*********************** PROGRAMME DE DIFFUSION DE LA CHALEUR *********************/


/**
 * Propagation de la chaleur selon l'axe x.
 * */
void diffuser_chaleur_x(MAT m) {
    for(int i = 0; i < TAILLE_MATRICE; i++) {
        for(int j = 0; j < TAILLE_MATRICE; j++) {
            m[i][j] = ((1/H) * mat[i][j - 1]) + ((4/H) * mat[i][j]) + ((1/H) * mat[i][j + 1]);
        }
        // Copie des valeurs de la nouvelle matrice dans mat
        for (int i = 0; i < TAILLE_MATRICE; i++) {
            for(int j = 0; j < TAILLE_MATRICE; j++) {
                mat[i][j] = m[i][j];
            }
        }
    }
}

// Propagation de la chaleur selon l'axe y
// NB : trouver une façon de factoriser les deux fonctions en une seule
void diffuser_chaleur_y(MAT m) {
    for(int j = 0; j < TAILLE_MATRICE; j++) {
        for(int i = 0; i < TAILLE_MATRICE; i++) {
            if (i == 0) {
                m[i][j] = ((4/H) * mat[i][j]) + ((1/H) * mat[i + 1][j]);
            } else if (i == TAILLE_MATRICE - 1) {
                m[i][j] = ((1/H) * mat[i - 1][j]) + ((4/H) * mat[i][j]);
            } else {
                m[i][j] = ((1/H) * mat[i - 1][j]) + ((4/H) * mat[i][j]) + ((1/H) * mat[i + 1][j]);
            }
        }
        // Copie des valeurs de la nouvelle matrice dans mat
        for (int i = 0; i < TAILLE_MATRICE; i++) {
            for(int j = 0; j < TAILLE_MATRICE; j++) {
                mat[i][j] = m[i][j];
            }
        }
    }
}

/**
 * Calcule et affiche le temps d'execution du programme
 * NB : les temps sont donnes en secondes. 
 * */
void calculer_temps_exec() {
    float temps[10];
    int taille_table = sizeof(temps)/sizeof(float);
    // Les min et max, que l'on determinera plus loin
    float min = 1.0;
    float max = 0.0;
    // Pour calculer la moyenne
    float moy = 0.0;

    clock_t temps_debut, temps_fin;
    // On lance les 10 executions
    for (int i = 0; i < 10; i++) {
        temps_debut = clock();

        // *** Debut de l'algorithme ***
        init();

        // On sauvegarde la matrice de depart, pour la prochaine execution
        for (int i = 0; i < TAILLE_MATRICE; i++) {
            for(int j = 0; j < TAILLE_MATRICE; j++) {
                current[i][j] = mat[i][j];
            }
        }

        for(int i = 0; i < NB_EXE; i++) {
            diffuser_chaleur_x(current);
            diffuser_chaleur_y(current);
        }

        // *** Fin de l'algoritme ***

        temps_fin = clock();
        // On stock le temps d'execution dans le tableau
        temps[i] = (float)(temps_fin - temps_debut)/CLOCKS_PER_SEC;

    }
    // On determine le min et le max dans le tableau
    for (int i = 0; i < taille_table; i++) {
        if(max < temps[i]) {
            max = temps[i];
        }
        if(min > temps[i]) {
            min = temps[i];
        }
    }
    // On fait la moyenne des temps restants
    for (int i = 0; i < taille_table; i++) {
        if(temps[i] != min && temps[i] != max) {
            moy += temps[i];
        }
    }

    // On fait la moyenne des 8 resultats d'execution
    moy /= (taille_table - 2);
    // On affiche le temps d'execution final
    printf("Temps d'execution (consommation du CPU) du programme : %f secondes\n", moy);
    afficher_options_synthetiques();
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

/**
 * Simule la diffusion de la chaleur en calculant le temps de reponse utilisateur.
 * */
void calculer_temps_user() {
    // Tableau contenant les 10 mesures de temps
    double tabTimes[10];    
    // Temps au debut de l'execution, temps a la fin
    double t_begin, t_end;
    // 10 executions du programme
    for (int i=0; i<10; i++) {
        t_begin = get_process_time();

        init();

        // for (int i = 0; i < TAILLE_MATRICE; i++) {
        //     for(int j = 0; j < TAILLE_MATRICE; j++) {
        //         current[i][j] = mat[i][j];
        //     }
        // }
        for(int i = 0; i < NB_EXE; i++) {
            diffuser_chaleur_x(current);
            diffuser_chaleur_y(current);
        }

        t_end = get_process_time();

        tabTimes[i] = (t_end - t_begin);
    }

    double max, min, moy;
    // On determine le min et le max dans le tableau
    for (int i = 0; i < 10; i++) {
        if(max < tabTimes[i]) {
            max = tabTimes[i];
        }
        if(min > tabTimes[i]) {
            min = tabTimes[i];
        }
    }
    // On fait la moyenne des temps restants
    for (int i = 0; i < 10; i++) {
        if(tabTimes[i] != min && tabTimes[i] != max) {
            moy += tabTimes[i];
        }
    }

    // On fait la moyenne des 8 resultats d'execution
    moy /= (10 - 2);
    // On affiche le temps d'execution final
    printf("Temps d'execution (reponse utilisateur) du programme : %f secondes\n", moy);
    afficher_options_synthetiques();
}

// Factorisation simple ?
void lancer_algo() {
    init();
    for(int i = 0; i < NB_EXE; i++) {
        diffuser_chaleur_x(current);
        diffuser_chaleur_y(current);
    }
    free_mat();
}


/**
 * Lance la procedure de repartition de la chaleur sur une nouvelle matrice.
 * */
void lancer_programme() {
    // Si l'utilisateur a demande le temps de reponse CPU
    if(MES_AFF_CPU) {
        calculer_temps_exec();
    } 

    // Si l'utilisateur a demande le temps de reponse utilisateur
    if(MES_AFF_REPUSER) {
        calculer_temps_user();
    }

    // Si l'utilisateur a demande l'affichage
    if(AFF) {
        printf("\n\n      Probleme de taille... %d\n",n);
        
        init();

        printf("Temperature initiale :\n");
        print_quarter_matrice(current);
        
        for(int i = 0; i < NB_EXE; i++) {
            diffuser_chaleur_x(current);
            diffuser_chaleur_y(current);
        }

        printf("\nTemperature finale :\n");
        print_quarter_matrice(current);
        free_mat();
    }
}




/**
 * Lance un nouvel algorithme pour chaque taille de matrice.
 * Initilialise TAILLE_MATRICE pour chaque n.
 * 
 * En effet a l'etape 0, seules les tailles de matrices font varier l'algorithme.
 * 
 * Lors des etapes a venir, cette fonction lancera un nouveau programme pour chaque :
 * - etape
 * - nombre de thread
 * - taille de matrice
 * 
 * Pour chacune de ces configurations, cette fonction initialisera 
 * ETAPES, NB_THREADS et TAILLE_MATRICE.
 * */
void lancer_selon_options() {
    while (*tailles++) {
        n = ((*(tailles-1)) - '0' )+4;
             
        TAILLE_MATRICE = (int) powi(2, n);

        // On a bien initialise ETAPES, NB_THREADS et TAILLE_MATRICE.
        // On lance le programme pour chacunes des configurations
        lancer_programme();
    }
}


/**
 * Programme principal.
 * 
 * Commence capter les potentielles options donnees par l'utilisateur.
 * 
 * Lance ensuite une fonction qui, elle, lancera le programme suivant toutes les configurations.
 * */
int main(int argc, char *argv[]) {
	if (argc != 1) {
		capter_options(argc,argv);
	}
    
    lancer_selon_options();

	return 0;
}
