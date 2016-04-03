#include "main.h"

/**
 * 
 * Projet de programmation concurrente : diffusion de la chaleur
 * SI4 2015/2016 - Info2
 * 
 * Auteurs : Arnaud Garnier & Lisa Joanno
 * 
 * Le programme suivant permet de visualiser le phenomene de diffusion de la chaleur sur une
 * plaque a deux dimensions.
 * 
 * Etape 0 rendue le 21/02/2016.
 * **/




/*********************** PROGRAMME DE DIFFUSION DE LA CHALEUR ***********************/




/**
 * Lancement de l'algorithme avec deux matrices.
 * */
void lancer_algo() {
    MAT mat_courante = init(TAILLE_MATRICE, n);
    MAT mat_prec = init(TAILLE_MATRICE, n);
    if (AFF) {
        printf("Matrice initiale : \n");
        print_matrice(mat_prec,TAILLE_MATRICE);        
    }
    if (ETAPE == 0 ) {
        int i;
        for(i = 0; i < NB_EXE; i++) {
            diffuser_chaleur_x(mat_courante,mat_prec,TAILLE_MATRICE);
            diffuser_chaleur_y(mat_prec,mat_courante,TAILLE_MATRICE);
            chauffer_zone_centrale(mat_prec, n);
        }
    } else if (ETAPE == 1) {
        int i;
        for(i = 0; i < NB_EXE; i++) {
                init_threads(mat_courante, mat_prec, n, TAILLE_MATRICE, NB_THREADS, t, 0);
        }
    } else if (ETAPE == 2) {
        int i;
        for(i = 0; i < NB_EXE; i++) {
                init_threads(mat_courante, mat_prec, n, TAILLE_MATRICE, NB_THREADS, t, 1);
        }
    }
    if (AFF) {
        printf("Matrice finale : \n");
        print_matrice(mat_prec,TAILLE_MATRICE);        
    }
    free_mat(mat_courante,TAILLE_MATRICE);
    free_mat(mat_prec,TAILLE_MATRICE);
}



/**
 * Calcule et affiche le temps d'execution du programme
 * NB : les temps sont donnes en secondes. 
 * */
void calculer_temps_cpu() {
    float temps[10];
    int taille_table = sizeof(temps)/sizeof(float);
    // Les min et max, que l'on determinera plus loin.
    float min = 1.0;
    float max = 0.0;
    // Pour calculer la moyenne.
    float moy = 0.0;
    clock_t temps_debut, temps_fin;
    // On lance les 10 executions.
    int i;
    for (i = 0; i < 10; i++) {
        temps_debut = clock();
        lancer_algo();
        temps_fin = clock();
        // On stocke le temps d'execution dans le tableau.
        temps[i] = (float)(temps_fin - temps_debut)/CLOCKS_PER_SEC;
    }
    // On determine le min et le max dans le tableau.
    for (i = 0; i < taille_table; i++) {
        if(max < temps[i]) {
            max = temps[i];
        }
        if(min > temps[i]) {
            min = temps[i];
        }
    }
    // On fait la moyenne des temps restants.
    for (i = 0; i < taille_table; i++) {
        if(temps[i] != min && temps[i] != max) {
            moy += temps[i];
        }
    }

    // On fait la moyenne des 8 resultats d'execution.
    moy /= (taille_table - 2);
    // On affiche le temps d'execution final.
    printf("Temps CPU consomme par le programme : %f secondes\n", moy);
    afficher_options_synthetiques(TAILLE_MATRICE,NB_EXE);
}



/**
 * Simule la diffusion de la chaleur en calculant le temps de reponse utilisateur.
 * */
void calculer_temps_user() {
    // Tableau contenant les 10 mesures de temps.
    double tabTimes[10];    
    // Temps au debut de l'execution, temps a la fin.
    double t_begin, t_end;
    // 10 executions du programme :

    int i;
    for (i=0; i<10; i++) {
        t_begin = get_process_time();
        lancer_algo();
        t_end = get_process_time();
        tabTimes[i] = (t_end - t_begin);
    }
    double max, min, moy;
    // On determine le min et le max dans le tableau.
    for (i = 0; i < 10; i++) {
        if(max < tabTimes[i]) {
            max = tabTimes[i];
        }
        if(min > tabTimes[i]) {
            min = tabTimes[i];
        }
    }
    // On fait la moyenne des temps restants.
    for (i = 0; i < 10; i++) {
        if(tabTimes[i] != min && tabTimes[i] != max) {
            moy += tabTimes[i];
        }
    }
    moy /= 8;
    // On affiche le temps d'execution final.
    printf("Temps d'execution (reponse utilisateur) du programme : %f secondes\n", moy);
    afficher_options_synthetiques(TAILLE_MATRICE,NB_EXE);
}

/**
 * Lance l'algorithme apres avoir affiche la taille du probleme.
 * **/
void lancer_algo_affichage() {
    printf("\n\n      Probleme de taille... %d\n",n);
    lancer_algo();
}



/**
 * Lance la procedure de repartition de la chaleur sur une nouvelle matrice.
 * */
void lancer_programme() {
    // Si l'utilisateur a demande le temps de reponse CPU :
    if(MES_AFF_CPU) {
        calculer_temps_cpu();
    } 

    // Si l'utilisateur a demande le temps de reponse utilisateur :
    if(MES_AFF_REPUSER) {
        calculer_temps_user();
    }

    // Si l'utilisateur a demande l'affichage :
    if(AFF) {
        lancer_algo_affichage();
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
 * - taille de matrice.
 * 
 * Pour chacune de ces configurations, cette fonction initialisera 
 * ETAPES, NB_THREADS et TAILLE_MATRICE.
 * */
void lancer_selon_options() {
    char* tempThreads = threads;
    char* tempTailles = tailles;

    // Parcours des etapes
    while (*etapes++)
    {
        ETAPE = (*(etapes-1)) - '0';
        printf("Execution de l'etape... %d\n",ETAPE);
        
        
        // Parcours des nombres de threads
        threads = tempThreads;
        while (*threads++)
        {
            t = (*(threads-1)) - '0';
            
            NB_THREADS = 1 << (2 * t);
            
            // Parcours des tailles de matrice
            tailles=tempTailles;
            while (*tailles++) {
                n = ((*(tailles-1)) - '0' )+4;
                TAILLE_MATRICE = (int) 1 << n;

                // On a bien initialise ETAPES, NB_THREADS et TAILLE_MATRICE.
                // On lance le programme pour chacunes des configurations.
                lancer_programme();
            }
            printf("\n");
        }
        printf("\n");
    }
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
    // Specification des options acceptees : -s, -r, -t, -i, -m, -M et -a :
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
            default:
                exit(EXIT_FAILURE);
        }
    }

    // Suppression de toute trace d'execution si -m ou -M :
    if(MES_AFF_REPUSER || MES_AFF_CPU) {
        AFF = 0;
    }
}




/**
 * Programme principal.
 * 
 * Commence capter les eventuelles options, donnees par l'utilisateur.
 * 
 * Execute ensuite une fonction qui lancera le programme en suivant 
 * toutes les configurations donnees par les options.
 * */
int main(int argc, char *argv[]) {
    if (argc != 1) {
        capter_options(argc,argv);
    }
    
    lancer_selon_options();

    return 0;
}
