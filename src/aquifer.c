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
float water_exch(float x2, float x3, float R){
    return x2 * pow(R/x3,(7.0/2.0));
}


/*
 * First update of the routing store level 
 */
float R_f1(float R, float Qa, float F){
    float dummy = R + Qa + F;
    if (dummy < 0){
        return 0;
    }else{
        return dummy;
    }
}

/*
 * Outflow from the routing store
 */
float Qr_f(float R, float x3){
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
float Qd_f(float Qb, float F){
    float dummy = Qb + F;
    if (dummy < 0){
        return 0;
    }else{
        return dummy;
    }
}

#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif




