#include "diffusion_chaleur.h"

// Constantes utilisee dans la formule de Taylor, fixee ici a 6
float H = 6;

/**
 * Propagation de la chaleur selon l'axe x.
 * */
void diffuser_chaleur_x(MAT m1, MAT m2, int taille) {
    for(int i = 0; i < taille; i++) {
        for(int j = 0; j < taille; j++) {
            m1[i][j] = ((m2[i][j - 1]) + (4 * m2[i][j]) + (m2[i][j + 1]))/H;
        }
    }
}



/**
 * Propagation de la chaleur selon l'axe y.
 * */
void diffuser_chaleur_y(MAT m1, MAT m2, int taille) {
    for(int j = 0; j < taille; j++) {
        for(int i = 0; i < taille; i++) {
            if (i == 0) {
                m1[i][j] = ((4 * m2[i][j]) + (m2[i + 1][j]))/H;
            } else if (i == taille - 1) {
                m1[i][j] = ((m2[i - 1][j]) + (4 * m2[i][j]))/H;
            } else {
                m1[i][j] = ((m2[i - 1][j]) + (4 * m2[i][j]) + (m2[i + 1][j]))/H;
            }
        }
    }
}