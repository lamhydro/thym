/*
 * =====================================================================================
 *
 *       Filename:  aquifer.h
 *
 *    Description:  Header file of aquifer.c 
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

#if MODEL == 1 // GR4J

typedef struct{
    float R, F, Qr, Qd;
} aqstore;
               
/*
 * Function prototypes
 */
float water_exch(float x2, float x3, float R);
float R_f1(float R, float Qa, float F);
float Qr_f(float R, float x3);
float Qd_f(float Qb, float F);

#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif




