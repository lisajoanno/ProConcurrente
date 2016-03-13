#include "thread_barriere.h"

void *thread(void *attr)
{
    struct thread_param *p = (struct thread_param*)(thread_param);
    int i;
    for(i = 0; i < NB_EXE; i++) {
	    diffuser_chaleur_x_ij(mat_courante, mat_prec, p->x_init, p->x_fin, p->y_init, p->y_fin);
	    // Attente des autres threads avant de commencer la propagation selon y
	    pthread_barrier_wait (&barrierX);
	    diffuser_chaleur_y_ij(mat_courante, mat_prec, p->x_init, p->x_fin, p->y_init, p->y_fin);
	    // Attente des autres threads avant prochaine iteration
	    pthread_barrier_wait (&barrierY);

        chauffer_zone_centrale(mat_prec, n);
    }

    // Synchronisation finale
	pthread_barrier_wait (&barrier);
}

void init_threads()
{
	pthread_t th[NB_THREAD];
	Thread_Param par[NB_THREAD];


    int pas = TAILLE_MATRICE;
    pas = TAILLE_MATRICE / (1 << nb_thread);

    int id = 0;
    int i;
	for (i = 0; i < N; i = i + (TAILLE_MATRICE/2)) {

		// Initialisation des coordonnées de départ et d'arrivée de
		// chaque thread
        int j;
        for (j = 0; j < N; j = j + (TAILLE_MATRICE/2)) {

        par[id].x_init = i;
        par[id].y_init = j;
        par[id].x_fin = i + pas;
        par[id].y_fin = j + (TAILLE_MATRICE/2);

        // Creation de la thread
        if(pthread_create(&th[id], NULL, thread, (void*)&p[id]))
        {
            printf("Impossible de creer la thread\n");
            return -1;
        }

        id++;

        }
    }

}

/* *
 * Test sur les threads ici
 *
 * */
int main()
{
	init_threads();
	return 0;
}

// NB : ajouter -lpthread pour compiler