/*
 * =====================================================================================
 *
 *       Filename:  soil_surf.c
 *
 *    Description:  Read thym's input files
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

#include "soil_surf.h"

#if MODEL == 1 // GR4J

/*
 * Percolation from the production store
 */
double percolation(double s, double x1){
    if (x1 == 0.0f) {
        printf("Error: Division by zero detected!\n");
        return NAN; // Return NaN (Not a Number) to indicate an error
    }
    return s*(1-pow(1+pow((4.0*s)/(9.0*x1),4.0),(-1.0/4.0)));
}

#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif




