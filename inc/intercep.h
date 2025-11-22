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

#ifndef INTERCEP_H  // Check if MY_HEADER_H is not defined
#define INTERCEP_H  // Define MY_HEADER_H
                   
#include <stdio.h>
#include <math.h>

#if MODEL == 1 // GR4J

typedef struct{
    double pn, en, ps, es;
} icstore;

/*
 * Function prototypes
 */
int pn_en(double p, double e, double *pn, double *en);
double ps_f(double pn, double x1, double s);
double es_f(double en, double x1, double s);

#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif

#endif // End of include guard
