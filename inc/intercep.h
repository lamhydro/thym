/*
 * =====================================================================================
 *
 *       Filename:  intercep.h
 *
 *    Description:  Header file of intercep.c 
 *
 *        Version:  1.0
 *        Created:  12/09/24 22:48:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (Luis Morales), 
 *   Organization:  UNAL
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <math.h>

#if MODEL == 1 // GR4J

typedef struct{
    float pn, en, ps, es;
} icstore;

/*
 * Function prototypes
 */
int pn_en(float p, float e, float *pn, float *en);
double ps_f(float pn, float x1, float s);
double es_f(float en, float x1, float s);

#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif


