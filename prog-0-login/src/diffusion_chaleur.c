#include "diffusion_chaleur.h"

// Constantes utilisee dans la formule de Taylor, fixee ici a 6
float H = 6;

/**
 * Propagation de la chaleur selon l'axe x.
 * */
void diffuser_chaleur_x(MAT m1, MAT m2, int taille) {
    int i;
    for(i = 0; i < taille; i++) {
        int j;
        for(j = 0; j < taille; j++) {
            if (j == 0) {
                m1[i][j] = ( (4 * m2[i][j]) + (m2[i][j + 1]) )/H;
            } else if (j == taille - 1) {
                m1[i][j] = ( (m2[i][j - 1]) + (4 * m2[i][j]) )/H;
            } else {            
                m1[i][j] = ( (m2[i][j - 1]) + (4 * m2[i][j]) + (m2[i][j + 1]) )/H;
            }
        }
    }
}



/**
 * Propagation de la chaleur selon l'axe y.
 * */
void diffuser_chaleur_y(MAT m1, MAT m2, int taille) {
    int j;
    for(j = 0; j < taille; j++) {
        int i;
        for(i = 0; i < taille; i++) {
            if (i == 0) {
                m1[i][j] = ( (4 * m2[i][j]) + (m2[i + 1][j]) )/H;
            } else if (i == taille - 1) {
                m1[i][j] = ( (m2[i - 1][j]) + (4 * m2[i][j]) )/H;
            } else {
                m1[i][j] = ( (m2[i - 1][j]) + (4 * m2[i][j]) + (m2[i + 1][j]) )/H;
            }
        }
    }
}



/**
 * Propagation de la chaleur selon l'axe x.
 * */
void diffuser_chaleur_x_ij(MAT m1, MAT m2, int i_in, int i_fi, int j_in, int j_fi) {
    int i;
    for(i = i_in; i < i_fi; i++) {
        int j;
        for(j = j_in; j < j_fi; j++) {
            if (j == (j_in)) {
                 m1[i][j] = ( (4 * m2[i][j]) + (m2[i][j + 1]) )/H;
             } else if (j == i_fi - 1) {
                 m1[i][j] = ( (m2[i][j - 1]) + (4 * m2[i][j]) )/H;
            } else {  
                m1[i][j] = ( (m2[i][j - 1]) + (4 * m2[i][j]) + (m2[i][j + 1]) )/H;
            }
        }
    }
}


/**
 * Propagation de la chaleur selon l'axe y.
 * */
void diffuser_chaleur_y_ij(MAT m1, MAT m2, int i_in, int i_fi, int j_in, int j_fi) {
    int j;
    for(j = j_in; j < j_fi; j++) {
        int i;
        for(i = i_in; i < i_fi; i++) {
            if (i == (i_in)) {
                m1[i][j] = ( (4 * m2[i][j]) + (m2[i + 1][j]) )/H;
            } else if (i == i_fi - 1) {
                m1[i][j] = ( (m2[i - 1][j]) + (4 * m2[i][j]) )/H;
            } else {
                m1[i][j] = ( (m2[i - 1][j]) + (4 * m2[i][j]) + (m2[i + 1][j]) )/H;
            }
        }
    }
}