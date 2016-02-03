#include <stdio.h>

// Tab sera le tableau de l'étape précédente
int Tab[7];

// on calcule récursivement T[x] et les valeurs
// de ses cases adjacentes
void calculer_cellule(int T[], int x) {
	T[x] = (1/6) * Tab[x - 1] + (4/6) * Tab[x] + (1/6) * Tab[x + 1];
}

// copie de deux tableaux
void copier_tableau(int T1[], int T2[]) {
	if(sizeof(T1) == sizeof(T2)) {
		for(unsigned int i = 0; i < sizeof(T1) / 4; i++) {
			T1[i] = T2[i];
		}
	}
}

int main()
{
	//int TabCourant[7];
	//copier_tableau(Tab, TabCourant);
	// le milieu s'obtient en divisant par 4
	// (un int est codé sur 4 octets) puis par 2
	Tab[sizeof(Tab)/8] = 256;
	calculer_cellule(Tab, sizeof(Tab) / 8 - 1);
	for(unsigned int i = 0; i < sizeof(Tab) / 4; i++) {
		printf("| %d ", Tab[i]);
	}
	//afficher_tableau(*TabCourant);
	printf("|\n");
	return 0;
}