/*
 * =====================================================================================
 *
 *       Filename:  intercept.c
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

#include "intercep.h"

#if MODEL == 1 // GR4J

/*
 * Estimate net precipitation and evapotranspiration
 */
int pn_en(double p, double e, double *pn, double *en){
    if (p >= e){
        *pn = p-e;
        *en = 0.0;
    }
    else{
        *en = e-p;
        *pn = 0.0;
    }

    return 0;
}

/*
 * Estimate efective precipitation
 */
double ps_f(double pn, double x1, double s){
    if (x1 == 0.0f) {
        printf("Error: Division by zero detected!\n");
        return NAN; // Return NaN (Not a Number) to indicate an error
    }
    return x1 * (1 - pow(s/x1,2.0)) * tanh(pn/x1) / (1 + ( (s/x1) * tanh(pn/x1)) );
}

/*
 * Estimate efective evapotranspiration 
 */
double es_f(double en, double x1, double s){
    if (x1 == 0.0f) {
        printf("Error: Division by zero detected!\n");
        return NAN; // Return NaN (Not a Number) to indicate an error
    }
    return s * (2 - (s/x1)) * tanh(en/x1) / (1 + ( (1-(s/x1)) * tanh(en/x1)) );
}

#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif


