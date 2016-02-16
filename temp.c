#include <stdio.h>
#include <stdlib.h> // pour malloc
#include <unistd.h> // pour getopt
#include <getopt.h> // pour getopt


/******************* PARAMETRES DU SYSTEME ****************************/

int TEMP_FROID = 0;
int TEMP_CHAUD = 256;

int n;
int TAILLE_MATRICE; // taille de la matrice entrée par l'utilisateur
int MES_AFF_CPU; // 1 : mesure et affichage du temps d'execution (consommation du CPU); 0 : non
int MES_AFF_REPUSER; // 1 : mesure et affichage du temps d'execution (temps de réponse utilisateur); 0 : non
int AFF; // 1 : affichage de la température initiale et de la température finale  pour les indices  correspondant au quart supérieur gauche du tableau; 0 : non
int NB_EXE; // nombre d'itérations à éxecuter
int ETAPES; // étapes du programme à exécuter
int NB_THREADS; // nombre de threads a créer

//~ #define taille pow(2,(TAILLE_MATRICE+4))

//~ typedef struct {
  //~ int taille;
  //~ float* data;
//~ } matrice;



char* etapes;
char* threads;
char* tailles;



/*********** DECLARATION & INITIALISATION DE LA MATRICE ****************/

typedef float **MAT;


//~ matrice* init_matrice() {
	//~ matrice* mat = malloc(sizeof(matrice));
	//~ mat->taille = TAILLE_MATRICE;
	//~ mat->data = malloc(mat->taille * mat->taille * sizeof(float));
	//~ int i;
	//~ for (i = 0; i < mat->taille*mat->taille; i++) {
    	//~ mat->data[i] = 0.0;
	//~ }
	//~ 
	//~ return mat;
//~ }

/**
 * Réimplémentation de la fonction pow à cause des problèmes causés par celle de la librairie math.h.
 * */
int powi(int number, int exponent)
{
	int i, product = 1;
	for (i = 0; i < exponent; i++)
		product *= number;

	return product;
}

/**
 * Alloue l'espace mémoire nécessaire à la matrice.
 * Initialise toutes les cases à 0 sauf la plaque interne.
 * */
MAT init() {
    // Allouer la place pour la matrice
	MAT mat =  malloc(sizeof(float) * TAILLE_MATRICE * TAILLE_MATRICE);
    // Initialisation de la matrice à 0 partout
    for (int i =0; i<TAILLE_MATRICE; i++) {
        for (int j=0; j<TAILLE_MATRICE; j++) {
            mat[i] = (float *) malloc(sizeof(float) * TAILLE_MATRICE);
            mat[i][j] = 0;
        }
    }
    
    // Initialisation de la zone chauffée initialement
    int idMin =  powi(2,n-1) - powi(2,n-4);
    int idMax = powi(2,n-1) + powi(2,n-4);
    printf("id min : %d\n",idMin);
    printf("id max : %d\n",idMax);
    for (int i =  idMin+1  ; i< idMax  ; i++) {
        for (int j=  idMin+1  ; j<  idMax ; j++) {
            mat[i][j] = TEMP_CHAUD;
        }
    }
    
    //~ mat[TAILLE_MATRICE/2][TAILLE_MATRICE/2] = TEMP_CHAUD;
    
	return mat;
}

/**
 * Affiche la matrice en paramètre.
 * */
void print_matrice(MAT m) {
    for (int i =0; i<TAILLE_MATRICE; i++) {
        for (int j=0; j<TAILLE_MATRICE; j++) {
            printf("%.1f ",m[i][j]);
        }
        printf("\n");
    }
}

/**
 * Affiche le quart supérieur gauche de la matrice en paramètre.
 * */
void print_quarter_matrice(MAT m) {
    for (int i =0; i<TAILLE_MATRICE/2; i++) {
        for (int j=0; j<TAILLE_MATRICE/2; j++) {
            printf("%.0f",m[i][j]);
        }
        printf("\n");
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
 * Initialisations par défaut des options du système (dans le cas où les options ne sont pas précisées par l'utilisateur).
 * */
void init_options_par_defaut() {
	//~ TAILLE_MATRICE = 24;
	MES_AFF_CPU = 1; 
	MES_AFF_REPUSER = 0;
	AFF = 0; 
	NB_EXE = 10000; 
	//~ ETAPES = 12345;
	//~ NB_THREADS = 13;
    
    tailles="024";
    //~ Pour les étapes suivantes : "012345"
    etapes="0";
    //~ Pour les étapes suivantes : "13"
    threads="1";
}

/**
 * Sauvegarde des options précisées par l'utilisateur dans les variables prévues à cet effet.
 * */
void capter_options(int argc, char *argv[]) {
	MES_AFF_CPU = 0;
	MES_AFF_REPUSER = 0;
	AFF = 0; 

	int option = 0;
    //Specifying the expected options
    //The two options l and b expect numbers as argument
    while ((option = getopt(argc, argv,"s:e:t:i:mMa")) != -1) {
        switch (option) {
            case 's' : 
             	//~ if (atoi(optarg) <= 9 && atoi(optarg) >= 0) {
             		//~ TAILLE_MATRICE = atoi(optarg);
                    tailles=optarg;
             	//~ }
                break;
            case 'e' : 
            	//~ if (atoi(optarg) <= 6 && atoi(optarg) >= 0) {
             		//~ ETAPES = atoi(optarg);
                    etapes=optarg;
             	//~ }
                break;
            case 't' : 
            	//~ if (atoi(optarg) <= 6 && atoi(optarg) >= 0) {
             		//~ NB_THREADS = atoi(optarg);
                    threads=optarg;
             	//~ }
                break;
            case 'i' : 
            	NB_EXE = atoi(optarg);
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


/**
 * Lance la procédure de répartition de la chaleur sur une nouvelle matrice.
 * */
void lancer_programme() {
    MAT mat = init();  
    print_quarter_matrice(mat);
}




/**
 * Lance un nouveau programme pour chaque :
 * - étape
 * - nombre de thread
 * - taille de matrice
 * 
 * Pour chacune de ces configurations, cette fonction initialise 
 * ETAPES, NB_THREADS et TAILLE_MATRICE.
 * */
void lancer_selon_options() {
    //~ printf("etapes : %s\n",etapes);
    //~ printf("threads : %s\n",threads);
    //~ printf("tailles : %s\n",tailles);
    
    // Temporaires pour garder les étapes, threads et tailles initiales.
    char* tempEtapes = etapes;
    char* tempThreads = threads;
    char* tempTailles = tailles;

    // Parcours des étapes
    while (*etapes++)
    {
        printf("Exécution de l'étape... %c\n",*(etapes-1));
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
                printf("      Problème de taille... %d\n",n);
                
                TAILLE_MATRICE = (int) powi(2, n);
                
                
                // On a bien initialisé ETAPES, NB_THREADS et TAILLE_MATRICE.
                // On lance le programme pour chacunes des configurations
                lancer_programme();
            }
            printf("\n");
        }
        printf("\n");
    }

}


/**
 * Programme principal.
 * 
 * Commance par initialiser les options, puis capte les potentielles options données par l'utilisateur.
 * 
 * Lance ensuite une fonction qui, elle, lancera le programme suivant toutes les configurations.
 * */
int main(int argc, char *argv[]) {
	printf("Programme de test\n");

	init_options_par_defaut();
	if (argc == 1) {
		printf("Il n'y a pas eu d'argument.\n");
	} else {
		printf("Il y a eu des arguments.\n");
		capter_options(argc,argv);
	}
    
    lancer_selon_options();

	return 0;
}
