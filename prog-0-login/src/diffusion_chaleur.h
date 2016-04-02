#ifndef _DIFFUSION_CHALEUR_H
#define _DIFFUSION_CHALEUR_H

#include "utils.h"

// Diffuse la chaleur sur l'axe x
void diffuser_chaleur_x(MAT,MAT,int);
// Diffuse la chaleur sur l'axe y
void diffuser_chaleur_y(MAT,MAT,int);

// Diffuse la chaleur sur l'axe x
void diffuser_chaleur_x_ij(MAT,MAT,int,int,int,int,int);
// Diffuse la chaleur sur l'axe y
void diffuser_chaleur_y_ij(MAT,MAT,int,int,int,int,int);
#endif
