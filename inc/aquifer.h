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
    double R, F, Qr, Qd;
} aqstore;
               
/*
 * Function prototypes
 */
double water_exch(double x2, double x3, double R);
double R_f1(double R, double Qa, double F);
double Qr_f(double R, double x3);
double Qd_f(double Qb, double F);

#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif




