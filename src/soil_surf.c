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

void soilModel(const double efprecip, const unsigned int i, const double fcap, const double lp, const double beta, const double et,  double *sowat, double *stw1, double *acet){

    double hsw, aet, runoff_depth;

    // starting point: equal to yesterday's storage
    sowat[i] = sowat[i-1];

    //If the soil moisture storage is already at capacity, runoff = all precip + excess
    if (sowat[i] >= fcap) {
        runoff_depth = efprecip + (sowat[i] - fcap);
        sowat[i] = fcap;
    }
    else
    {
        //This is the portion of the effective precip that goes into storage
        hsw = efprecip * (1.0 - pow((sowat[i]/fcap), beta));
        sowat[i] += hsw;
        runoff_depth = efprecip - hsw;

        //If the amount going into the soil moisture storage will result in exceeding the capacity of the store...
        if (sowat[i] > fcap)
        {
            runoff_depth += (sowat[i] - fcap);
            sowat[i] = fcap; //We are at capacity
        }
    }

    aet = et*fmin(sowat[i-1]/(fcap*lp), 1.0); // actual ET, after adjusting for saturation in soil layer
    if (aet < 0.0) aet = 0.0;

    //If there is enough in the soil moisture store to supply the aet, subtract it
    if (sowat[i] > aet) {
        acet[i] = aet;
        sowat[i] -= aet;
    }
    else {
        acet[i] = sowat[i];
        sowat[i] = 0.0; // all of it evaporates
    }

    stw1[i] += stw1[i-1] + runoff_depth;

}

#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif




