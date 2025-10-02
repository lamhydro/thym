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
#include <math.h>  

#define PA 0.9 // Proportion of flow toward the routing store
#define PB 0.1 // Proportion of flow toward ...
               
#if MODEL == 1 // GR4J
               
typedef struct{
    float Qa, Qb;
} chstore;


/*
 * Function prototypes
 */
int uh1_f(int x4, float *uh1);
int uh2_f(int x4, float *uh2);
float Qa_f(float Pr, float *uh1, int x4);   
float Qb_f(float Pr, float *uh2, int x4);   

#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif




