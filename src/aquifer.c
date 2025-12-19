/*
 * =====================================================================================
 *
 *       Filename:  aquifer.c
 *
 *    Description:  
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
#include "aquifer.h"
#include <math.h>
              
#if MODEL == 1 // GR4J
/*
 * Catchment water exchange
 */
double water_exch(double x2, double x3, double R){
    return x2 * pow(R/x3,(7.0/2.0));
}


/*
 * First update of the routing store level 
 */
double R_f1(double R, double Qa, double F){
    double dummy = R + Qa + F;
    if (dummy < 0){
        return 0;
    }else{
        return dummy;
    }
}

/*
 * Outflow from the routing store
 */
double Qr_f(double R, double x3){
    if (x3 == 0.0f) {
        printf("Error: Division by zero detected!\n");
        return NAN; // Return NaN (Not a Number) to indicate an error
    }
    //return R*(1 - (1 + (R/x3)**4.0)**(-1.0/4.0));
    return R*(1 - pow(1 + pow(R/x3,4.0),(-1.0/4.0)));
}

/*
 * Qd
 */
double Qd_f(double Qb, double F){
    double dummy = Qb + F;
    if (dummy < 0){
        return 0;
    }else{
        return dummy;
    }
}

#elif MODEL == 2 // HBV

    double dischargeModel(const unsigned int i, double *stw1, double *stw2, const double hl1, const double k0, const double k1, const double k2, const double perc)
    {
    
        double Q0 = 0.0;
        double Q1 = 0.0;
        double Q2 = 0.0;
        double Qall = 0.0;
    
        //If the upper reservoir water level is above the threshold for near surface flow
        if (stw1[i] > hl1)
        {
            //Calculate it, and remove it from the reservoir
            Q0 = (stw1[i] - hl1)*k0;
            stw1[i] -= Q0;
        }
        else Q0 = 0.0;
    
        //If there is still water left in the upper reservoir
        if (stw1[i] > 0.0)
        {
            //Calculate what now goes into interflow, and remove it
            Q1 = stw1[i] * k1;
            stw1[i] -= Q1;
        }
        else Q1 = 0.0;
    
        //If there is still anough water in the upper reservois to completely supply percolation...
        if (stw1[i] > perc)
        {
            // Move the amount from the upper to the lower reservoir
            stw1[i] -= perc;
            stw2[i] += perc;
        }
        else
        {
            //We just put what we can from the upper into the lower
            stw2[i] += stw1[i];
            stw1[i] = 0.0;
        }
    
        //If there is water in the lower reservoir...
        if (stw2[i] > 0.0)
        {
            //Calculate base flow, and remove it
            Q2 = stw2[i] * k2;
            stw2[i] -= Q2;
        }
        else Q2 = 0.0;
    
        Qall = Q0 + Q1 + Q2; // total dischargearge - mm per timestep
        return Qall;
    }
    
#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif




