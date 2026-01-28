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

#ifndef SOIL_SURF_H  // Check if MY_HEADER_H is not defined
#define SOIL_SURF_H  // Define MY_HEADER_H
                    
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

    void soilModel(const double efprecip, const unsigned int i, const double fcap, const double lp, const double beta, const double et,  double *sowat, double *stw1, double *acet);

#elif MODEL == 3 // HYMOD

    void soilModel(const double efprecip, const unsigned int i, const double cpar, const double huz, const double b, const double et, const double kv, double *xhuz, double *ov, double *ae, double *xcuz);

#else // IAHCRES


#endif

#endif // End of include guard
