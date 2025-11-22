/*
 * =====================================================================================
 *
 *       Filename:  model.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/03/25 22:33:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "model.h"

#if MODEL == 1 // GR4J

//int gr4j(float p, float e, float x1, float x2, float x3, int x4){
/* int gr4j(meteoin *metin, evapot *evp, modparam modp, int ntimes){ */
/* int gr4j(const double *precip, const double *et, const modparam *modp, const int ntimes, modstvar *mostv){ */
int gr4j(const double *rainfall, const double *snomlt, const double *eres, const modparam *modp, const int ntimes, modstvar *mostv){

/*         double pn; */
        /* double ps; */
        /* double pr; */
        /* double en; */
        /* double es; */
        /* double s; */
        /* double r; */
        /* double perc; */
        /* double f; */
        /* double qa; */
        /* double qb; */
        /* double qr; */
        /* double qd; */
        /* double q; */
 
    /* double p, e, pn, en, ps, es, s, perc, pr, f, r, qr, qa, qb, qd; */
    double p, e;

    unsigned int i;
    double s0 = 0.2*modp->x1; // Initializing the production store level
    /* double dummy = 0.001*area/dt; */
    for(i = 0; i < ntimes; i++){ // Temporal loop
        p = rainfall[i]+snomlt[i];
        e = eres[i];

        // INTERCEPTION
        // Net precipitation and evapotranspiration 
        pn_en(p, e, &mostv[i].pn, &mostv[i].en);
        if (p >= e){
            // Precipitation added to the production store
            mostv[i].ps = ps_f(mostv[i].pn, modp->x1, s0);
            mostv[i].es = 0.0;
        }else{
            // Evapotranspiration added to the production store
            mostv[i].es = es_f(mostv[i].en, modp->x1, s0);
            mostv[i].ps = 0.0;
        }
        
        // SOIL SURFACE
        // Update water level in the production store
        mostv[i].s = s0 - mostv[i].es + mostv[i].ps;
        /* Note that s must be less or equal to modp->x1 */
        if (mostv[i].s > modp->x1){ 
            mostv[i].s = modp->x1;
        }
        // Estimate percolation from the production store
        mostv[i].perc = percolation(mostv[i].s, modp->x1);
        // Final update of water level in the production store
        mostv[i].s = mostv[i].s - mostv[i].perc;
        /* Note that s must be less or equal to modp->x1 */
        if (mostv[i].s > modp->x1){
            mostv[i].s = modp->x1;
        }
        // Water height that reach the routing functions
        mostv[i].pr = mostv[i].perc + mostv[i].pn - mostv[i].ps;
        s0 = mostv[i].s;
    }

    // Array of UH1 values
    double *uh1;
    int uh1_l;
    uh1_f(modp->x4, &uh1, &uh1_l);
    print_uh(uh1, uh1_l);

    // Array of UH2 values
    double *uh2;
    int uh2_l;
    uh2_f(modp->x4, &uh2, &uh2_l);
    print_uh(uh2, uh2_l);

    // ROUTING
    double r0 = 0.1*modp->x3; // Initializing the routing store level
    unsigned int t;
    for(i = 0; i < ntimes; i++){ // Temporal loop
        mostv[i].qb = 0.0;
        mostv[i].qa = 0.0;
        if (i + 1 >= uh2_l)
        {
            for (t = 0; t < uh1_l; t++)
            {
                mostv[i].qa += *(uh1 + t) * mostv[i-t].pr * PA; 
            }
            for (t = 0; t < uh2_l; t++)
            {
                mostv[i].qb += *(uh2 + t) * mostv[i-t].pr * PB;
            }
        } else if (i + 1 >= uh1_l && i + 1 < uh2_l)
        {
            for (t = 0; t < uh1_l; t++)
            {
                mostv[i].qa += *(uh1 + t) * mostv[i-t].pr * PA; 
            }
            for (t = 0; t < i + 1; t++)
            {
                mostv[i].qb += *(uh2 + t) * mostv[i-t].pr * PB;
            }
        } else if (i + 1 < uh1_l)
        {
            for (t = 0; t < i + 1; t++)
            {
                mostv[i].qa += *(uh1 + t) * mostv[i-t].pr * PA; 
            }
            for (t = 0; t < i + 1; t++)
            {
                mostv[i].qb += *(uh2 + t) * mostv[i-t].pr * PB;
            }
        }
       
        // Catchment water exchage
        mostv[i].f = water_exch(modp->x2, modp->x3, r0);

        // Inflow to the routing store
        /* qa = Q_f(pr, uh1, uh1_l, PA);    */
        
        // Update the routing store water level
        mostv[i].r = R_f1(r0, mostv[i].qa, mostv[i].f);
        
        // Outflow from the routing store
        mostv[i].qr = Qr_f(mostv[i].r, modp->x3);
        
        // Final update the routing store water level
        mostv[i].r = mostv[i].r - mostv[i].qr;
        /* Note that r must be less or equal to modp->x3 */
        if (mostv[i].r > modp->x3){
            mostv[i].r = modp->x3;
        }
       
        // Direct outflow
        /* qb = Q_f(pr, uh2, uh2_l, PB);    */
        mostv[i].qd = Qd_f(mostv[i].qb, mostv[i].f);
        
        // Total streamflow
        /* runoff_s[i] = (qr + qd)*dummy; // Discharge in m^3/s */
        mostv[i].q = (mostv[i].qr + mostv[i].qd); // Discharge in m^3/s
        /* printf("The streamflow is:%f\n",runoff_s[i]); */

        r0 = mostv[i].r; // Updating the past routing store level
    }
    return 0;
}

#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif




