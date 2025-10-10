/*
 * =====================================================================================
 *
 *       Filename:  channel.h
 *
 *    Description:  Header file of channel.c 
 *
 *        Version:  1.0
 *        Created:  12/09/24 22:43:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>  

#define PA 0.9 // Proportion of flow toward the routing store
#define PB 0.1 // Proportion of flow toward ...
               
#if MODEL == 1 // GR4J
               
typedef struct{
    double Qa, Qb;
} chstore;


/*
 * Function prototypes
 */
int uh1_f(double x4, double **uh, int *uh_l);
int uh2_f(double x4, double **uh, int *uh_l);
double Q_f(double Pr, double *uh, int uh_l, float per);   

#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif




