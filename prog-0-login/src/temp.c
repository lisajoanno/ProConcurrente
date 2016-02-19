#include <stdio.h>
#include <stdlib.h> // pour malloc
#include <unistd.h> // pour getopt
#include <getopt.h> // pour getopt

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
int TEMP_CHAUD = 36;
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

// On definit deux matrices
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
MAT init() {
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

	return mat;
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
 * Initialisations par defaut des options du systeme (dans le cas ou les options ne sont pas precisees par l'utilisateur).
 * Fonction normalement inutilisee car les options doivent etre precisees mais utile en phase de developpement.
 * */
void init_options_par_defaut() {
	MES_AFF_CPU = 1; 
	MES_AFF_REPUSER = 0;
	AFF = 0; 
	NB_EXE = 10000; 
    
    //~ Pour les etapes suivantes : "024"
    tailles="024";
    //~ Etape 0 disponible
    etapes="0";
    //~ A l'etape 0, pas de thread
    threads="1";
}





/**
 * Sauvegarde des options precisees par l'utilisateur dans les variables prevues a cet effet.
 * */
void capter_options(int argc, char *argv[]) {
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
}

// Propagation de la chaleur selon l'axe x
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
 * Lance la procedure de repartition de la chaleur sur une nouvelle matrice.
 * */
void lancer_programme() {
    printf("Dans lancer programme");
    init();
    
    afficher_options();

    if (AFF == 1) {
        printf("Temperature initiale :\n");
        print_matrice(mat);
    }
    
    for(int i = 0; i < NB_EXE; i++) {
        diffuser_chaleur_x(current);
        diffuser_chaleur_y(current);
    }
    
    if (AFF) {
        printf("Temperature finale :\n");
        print_matrice(mat);
    }   
    free(mat);
}




/**
 * Lance un nouveau programme pour chaque :
 * - etape
 * - nombre de thread
 * - taille de matrice
 * 
 * Pour chacune de ces configurations, cette fonction initialise 
 * ETAPES, NB_THREADS et TAILLE_MATRICE.
 * */
void lancer_selon_options() {
    
    /**
    
    // Temporaires pour garder les etapes, threads et tailles initiales.
    //~ char* tempEtapes = etapes;
    char* tempThreads = threads;
    char* tempTailles = tailles;
    
    //~ printf("etape %s\n",etapes);
    //~ printf("thread %s\n",threads);
    //~ printf("tailles %s\n",tailles);

    // Parcours des etapes
    while (*etapes++)
    {
        
        printf("Execution de l'etape... %c\n",*(etapes-1));
        ETAPES = (*(etapes-1));
        
        // Parcours des nombres de threads
        threads = tempThreads;
        while (*threads++)
        {
            printf("   Nombre de threads... %c\n",*(threads-1));
            NB_THREADS = (*(threads-1));
            
            // Parcours des tailles de matrice
            tailles=tempTailles;
            while (*tailles++)
            {
                n = ((*(tailles-1)) - '0' )+4;
                printf("      Probleme de taille... %d\n",n);
                
                TAILLE_MATRICE = (int) powi(2, n);
                printf("      Probleme de TAILLE_MATRICE... %d\n",TAILLE_MATRICE);
                
                
                
                // On a bien initialise ETAPES, NB_THREADS et TAILLE_MATRICE.
                // On lance le programme pour chacunes des configurations
                lancer_programme();
            }
            printf("\n");
        }
        printf("\n");
    }
    
    **/
    
            while (*tailles++)
            {
                n = ((*(tailles-1)) - '0' )+4;
                printf("      Probleme de taille... %d\n",n);
                
                TAILLE_MATRICE = (int) powi(2, n);
                printf("      Probleme de TAILLE_MATRICE... %d\n",TAILLE_MATRICE);
                
                // On a bien initialise ETAPES, NB_THREADS et TAILLE_MATRICE.
                // On lance le programme pour chacunes des configurations
                lancer_programme();
            }

}


/**
 * Programme principal.
 * 
 * Commence par initialiser les options, puis capte les potentielles options donnees par l'utilisateur.
 * 
 * Lance ensuite une fonction qui, elle, lancera le programme suivant toutes les configurations.
 * 
 * Libere la memoire a la fin de l'execution du programme.
 * */
int main(int argc, char *argv[]) {
	printf("Programme de simulation de la diffusion de la chaleur\n");
    
	init_options_par_defaut();
	if (argc != 1) {
		capter_options(argc,argv);
	}
    
    lancer_selon_options();

	return 0;
}
