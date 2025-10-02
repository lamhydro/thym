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
int gr4j(float *precip, float *et, float x1, float x2, float x3, int x4, int ntimes, float area, int dt, float *disch_s){

/*     float x1 = modp.x1; */
    /* float x2 = modp.x2; */
    /* float x3 = modp.x3; */
    /* int x4 = modp.x4; */

    float p, e, pn, en, ps, es, s, perc, pr, f, r, qr, qa, qb, qd;
    float uh1[x4], uh2[2*x4];

    // Array of UH1 values
    uh1_f(x4, uh1);

    // Array of UH2 values
    uh2_f(x4, uh2);

    s = 0.0; // Initializing the production store level
    r = 0.0; // Initializing the routing store level

    unsigned int i;
    float dummy = 0.001*area/dt;
    for(i=0; i<ntimes; i++){ // Temporal loop
        p = precip[i];
        e = et[i];

        // INTERCEPTION
        // Net precipitation and evapotranspiration 
        pn_en(p,  e,  &pn,  &en);

        // Precipitation added to the production store
        ps = ps_f(pn, x1, s);

        // Evapotranspiration added to the production store
        es = es_f(en, x1, s);
        
        // SOIL SURFACE
        // Update water level in the production store
        s = s - es + ps;
        /* Note that s must be less or equal to x1 */
        if (s>x1){ 
            s=x1;
        }

        // Estimate percolation from the production store
        perc = percolation(s, x1);

        // Final update of water level in the production store
        s = s - perc;
        /* Note that s must be less or equal to x1 */
        if (s>x1){
            s=x1;
        }

        // ROUTING
        // Water height that reach the routing functions
        pr = perc + pn - ps;
        
        // Catchment water exchage
        f = water_exch(x2, x3, r);

        // Inflow to the routing store
        qa = Qa_f(pr, uh1, x4);   
        
        // Update the routing store water level
        r = R_f1(r, qa, f);
        
        // Outflow from the routing store
        qr = Qr_f(r, x3);
        
        // Final update the routing store water level
        r = r - qr;
        /* Note that r must be less or equal to x3 */
        if (r>x3){
            r=x3;
        }
       
        // Direct outflow
        qb = Qb_f(pr, uh2, x4);   
        qd = Qd_f(qb, f);
        
        // Total streamflow
        disch_s[i] = (qr + qd)*dummy; // Discharge in m^3/s
        /* printf("The streamflow is:%f\n",disch_s[i]); */
        
        i++;
    }
    return 0;
}

#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif




