#include "thread_barriere.h"

void *thread(void *attr)
{
	diffuser_chaleur_x_ij(mat_courante, mat_prec, *(int*) attr, *(int*) attr + delta);
	// <--- Insérer barrière --->
	diffuser_chaleur_y_ij(mat_courante, mat_prec, *(int*) attr, *(int*) attr + delta);
	// <--- Insérer barrière --->
}

void init_threads()
{
	pthread_t th;

	for (int i = 0; i < NB_THREAD; i++) {
		int n = 0;
		int delta = TAILLE_MATRICE/2;
		pthread_create(&th, NULL, thread, &n);
		n += pas;
		delta = TAILLE_MATRICE;
	}
}

int main()
{
	init_threads();
	// <--- Insérer barrière --->
	return 0;
}

// NB : ajouter -lpthread pour compiler