/**
 *
 * Auteur : Arnaud Garnier
 *
 * Implementation d'un equivalent de la barriere posix.
 *
 */

#include "barriere.h"

/**
 * Detruit la barriere : pour cela, on detruit la variable de
 * condition puis le mutex.
 *
 */
 void detruire_barriere(custom_barriere* barriere)
{
 	if (pthread_cond_destroy(&barriere->var_cond))
 	{
 		printf("Erreur lors de la destruction de la condition\n");
 		exit(1);
 	}

 	if (pthread_mutex_destroy(&barriere->mutex))
 	{
 		printf("Erreur lors de la destruction du mutex\n");
 		exit(1);
 	}
 }

/**
 * Initialise les champs de la barriere.
 *
 */
 void init_barriere(custom_barriere* barriere, unsigned nb_thread)
 {
 	barriere->nb_thread = nb_thread;
	// On initialise le compteur a 0 par defaut
 	barriere->count = 0;
 	if(pthread_mutex_init(&barriere->mutex, NULL))
 	{
 		printf("Erreur lors de l'initialisation du mutex\n");
 		exit(1);
 	}
 	if (pthread_cond_init(&barriere->var_cond, NULL))
 	{
 		printf("Erreur lors de l'initialisation de la variable de condition\n");
 		exit(1);
 	}
 }

/**
 * Simule le comportement du 'wait' de la barriere posix.
 * Permet d'attendre la fin de toutes les threads.
 *
 */
 int barriere_wait(custom_barriere* barriere)
 {
 	if(pthread_mutex_lock(&barriere->mutex))
 	{
 		printf("Erreur lors du lock du mutex\n");
 		exit(1);
 	}
	// On incremente le compteur
 	barriere->count += 1;
 	// Si on a atteint le nombre maximal de threads :
	 if (barriere->count == barriere->nb_thread)
	 {
	 	barriere->count = 0;
	 	// Broadcast
		if (pthread_cond_broadcast(&barriere->var_cond))
		{
			printf("Erreur pthread_cond_broadcast\n");
			exit(1);
		}
		// On deverouille le mutex
		if (pthread_mutex_unlock(&barriere->mutex))
		{
			printf("Erreur unlock mutex\n");
			exit(1);
		}
		return PTHREAD_BARRIER_SERIAL_THREAD;
	} else {
		// Sinon, on attend que toutes les threads soient arrivees
		while(barriere->count != 0)
		{
			if (pthread_cond_wait(&barriere->var_cond, &barriere->mutex))
			{
				printf("Erreur pendant le wait de var_cond\n");
				exit(1);
			}
		}
		if (pthread_mutex_unlock(&barriere->mutex))
		{
			printf("Erreur unlock mutex\n");
			exit(1);
		}

	return 0;

	}

}