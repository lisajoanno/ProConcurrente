/**
 *
 * Auteur : 
 *
 * Implementation de thread et synchronisation de celles-ci
 * avec la barriere posix, puis par notre implementation de
 * la barriere. 
 *
 */

#include "thread_barriere.h"


void *thread(void *attr)
{
    ThreadParam *p = (ThreadParam*) attr;
    diffuser_chaleur_x_ij(p->mat_courante, p->mat_prec, p->x_init, p->x_fin, p->y_init, p->y_fin,p->taille);

    // Attente des autres threads avant de commencer la propagation selon y
    if (p->custom_barriere)
    {
        barriere_wait(&c_barrierX);
    }
    else
    {
        pthread_barrier_wait (&barrierX);
    }
    diffuser_chaleur_y_ij(p->mat_prec, p->mat_courante, p->x_init, p->x_fin, p->y_init, p->y_fin,p->taille);

    // Attente des autres threads avant prochaine iteration
    if (p->custom_barriere)
    {
        barriere_wait(&c_barrierY);
    }
    else
    {
        pthread_barrier_wait (&barrierY);
    } 

    chauffer_zone_centrale(p->mat_prec, p->n);

    // Synchronisation finale
    if (p->custom_barriere)
    {
        barriere_wait(&c_barrier);
    }
    else
    {
        pthread_barrier_wait(&barrier);
    }
    
    pthread_exit(NULL);
}

void init_threads(MAT mat_courante, MAT mat_prec, int n, int taille, int nb_thread, int t, int custom_barriere)
{
    pthread_t th[nb_thread];
    ThreadParam par[nb_thread];

    if (nb_thread > (taille * taille))
        exit(0);

    int pas = taille;
    pas = taille / (1 << t);

    int id = 0;
    int i = 0;

    if (custom_barriere) 
    {
        init_barriere(&c_barrierX, nb_thread);
        init_barriere(&c_barrierY, nb_thread);
        init_barriere(&c_barrier, nb_thread);
    }
    else
    {
        if(pthread_barrier_init(&barrierX, NULL, nb_thread))
        {
            printf("Impossible de créer la barriere\n");
        }
        if(pthread_barrier_init(&barrierY, NULL, nb_thread))
        {
            printf("Impossible de créer la barriere\n");
        }     
        if(pthread_barrier_init(&barrier, NULL, nb_thread))
        {
            printf("Impossible de créer la barriere\n");
        }
    }

    for (i = 0; i < taille; i = i + pas)
    {
        // Initialisation des coordonnées de départ et d'arrivée de
        // chaque thread
        int j = 0;
        for (j = 0; j < taille; j = j + pas)
        {    
            par[id].n = n;
            par[id].mat_prec = mat_prec;
            par[id].mat_courante = mat_courante;
            par[id].x_init = i;
            par[id].y_init = j;
            par[id].x_fin = i + pas;
            par[id].y_fin = j + pas;
            par[id].taille = taille;
            par[id].custom_barriere = custom_barriere;
            // Creation de la thread
            if(pthread_create(&th[id], NULL, thread, (void*)&par[id]))
            {
                printf("Impossible de creer la thread\n");
            }
            
            id++;
        }
    }

    int k = 0;
    for (k = 0; k < nb_thread; k++)
    {
        pthread_join(th[k],NULL);
    }

    if (custom_barriere)
    {
        detruire_barriere(&c_barrierX);
        detruire_barriere(&c_barrierY);
        detruire_barriere(&c_barrier);
        
    }
    else
    {
        if(pthread_barrier_destroy(&barrierX)){
            perror("Destruction de la barriere impossible");
        }
        if(pthread_barrier_destroy(&barrierY)){
            perror("Destruction de la barriere impossible");
        }
        if(pthread_barrier_destroy(&barrier)){
            perror("Destruction de la barriere impossible");
        }
    }

}
