/*
 * =====================================================================================
 *
 *       Filename:  evapotrans.c
 *
 *    Description:  Functions to estimate potential evapotranspiration.
 *
 *        Version:  1.0
 *        Created:  14/09/24 07:35:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Luis Morales-Marin, 
 *   Organization:  Universidad Nacional de Colombia
 *
 * =====================================================================================
 */


#include "evapotrans.h"

/*
 * Hamon method to estimate potential evapotranspiration
 */
double hamon_et(double Tave, double latr, int jday){

    // Solar declination angle (radians)
    double d = 1 + 0.033*cos(2*M_PI*jday/365);

    // Sunset hour angle
    double w = acos(-tan((double)latr)*tan(d)); 

    // Number of daylight hours
    double N = 24*w/M_PI;

    // Saturation vapor pressure es at the mean daily temperature 
    double e_s = 0.6108*exp(17.27*(double)Tave/((double)Tave+237.3));

    // Saturation vapor density
    double Pt = 216.7*e_s/(Tave+273.16);

    return HAMON_C*N*Pt/12;
}


/*
 * Hargreaves method to estimate potential evapotranspiration
 */
/* float hargreaves_et(){ */
/* } */


/*
 *  Potential evapotranspiration for multiple input data
 */
/* int ets(evapot *evp, meteoin *metin, float lat, int ntimes){ */
int ets(char *etmethod, double *tave, struct tm *timestamp, double lat, int ntimes, double *et){

    if (strcmp(etmethod, "hamon") == 0) {
        printf("ET computed using Hamon method\n");
        
        // Latitude to radians
        double latr = lat * M_PI / 180.0;
        
        // Estimation of ET 
        /* float Tave; */
        int yday;
        unsigned int i;
        for(i = 0; i<ntimes; i++){
            /* Tave = metin[i].tave; */
            /* jday = metin[i].timestamp.tm_yday+1;  */
            yday = timestamp[i].tm_yday+1;// +1 because the counter of yday start at 0. */
            et[i]= hamon_et(tave[i], latr, yday); 
        }

/*     } else if (strcmp(etmethod, "hargreaves") == 0) { */
        /* printf("ET computed using Hargreaves method\n"); */

    } else {
        printf("ET observations\n");
    }
    return 0;
}
