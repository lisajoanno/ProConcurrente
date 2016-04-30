/**
 *
 * Auteur : Arnaud Garnier
 *
 * Simulation de la barriere posix
 *
 */

#ifndef _BARRIERE_H_
#define _BARRIERE_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Structure de la barriere
typedef struct {
	unsigned nb_thread;
	unsigned count;
	pthread_mutex_t mutex;
	pthread_cond_t var_cond;
} custom_barriere;

// Detruit la barriere
void detruire_barriere(custom_barriere* barriere);

// Initialise la barriere
void init_barriere(custom_barriere* barriere, unsigned nb_thread);

// Simule l'action 'wait' de la barriere posix
int barriere_wait(custom_barriere* barriere);

#endif
