/*
 * =====================================================================================
 *
 *       Filename:  soil_surf.h
 *
 *    Description:  Header file of soil_surf.c 
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
    double perc, s;
} ssstore;

/*
 * Function prototypes
 */
double percolation(double s, double x1);

#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif




