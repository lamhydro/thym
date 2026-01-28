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

/* int gr4j(const double *rainfall, const double *snomlt, const double *eres, const modparam *modp, const unsigned int ntimes, modstvar *mostv){ */

    void gr4j(const unsigned int ntimes, const double *tav, const double *precip, const double *tmax, const double *et, const modparam *modp, modstatev *mstv, modfluxv *mfxv, struct tm *timestamp){

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

    /* Snow modelling variables */
    double sno0 = 5;
    double tsnow0 = -5;
    /* double ratio; */
    /* double cov1, cov2; */
    double ratio = modp->sno50 / modp->sno100;
    double cov2 = log(0.05 / ratio) / (ratio - 0.95);
    double cov1 = log(0.05) + 0.95 * cov2;
    for(i = 0; i < ntimes; i++){ // Temporal loop

        // SNOW MODELLING
        if (tav[i] >= modp->trs){
                /* No snow */
                mfxv->rainfall[i] = precip[i];
                mfxv->snowfall[i] = 0.0;
            } else {
                /* Snow */
                mfxv->snowfall[i] = precip[i];
                mfxv->rainfall[i] = 0.0;

            /* efprecip = snowModel(i, tav[i], precip[i], mstv->sdep, modp->ttlim, modp->degw, modp->degd); */
        }

        snowModel(i, timestamp, mfxv->snowfall, tav, tmax, et, mstv->tsnow, mfxv->sno, mfxv->snomlt, mfxv->eres, modp, cov1, cov2, &sno0, &tsnow0);

        // INTERCEPTION
        p = mfxv->rainfall[i] + mfxv->snomlt[i];
        e = mfxv->eres[i];
        // Net precipitation and evapotranspiration 
        pn_en(p, e, &mfxv->pn[i], &mfxv->en[i]);
        if (p >= e){
            // Precipitation added to the production store
            mfxv->ps[i] = ps_f(mfxv->pn[i], modp->x1, s0);
            mfxv->es[i] = 0.0;
        }else{
            // Evapotranspiration added to the production store
            mfxv->es[i] = es_f(mfxv->en[i], modp->x1, s0);
            mfxv->ps[i] = 0.0;
        }
        
        // SOIL SURFACE
        // Update water level in the production store
        mstv->s[i] = s0 - mfxv->es[i] + mfxv->ps[i];
        /* Note that s must be less or equal to modp->x1 */
        if (mstv->s[i] > modp->x1){ 
            mstv->s[i] = modp->x1;
        }
        // Estimate percolation from the production store
        mfxv->perc[i] = percolation(mstv->s[i], modp->x1);
        // Final update of water level in the production store
        mstv->s[i] = mstv->s[i] - mfxv->perc[i];
        /* Note that s must be less or equal to modp->x1 */
        if (mstv->s[i] > modp->x1){
            mstv->s[i] = modp->x1;
        }
        // Water height that reach the routing functions
        mfxv->pr[i] = mfxv->perc[i] + mfxv->pn[i] - mfxv->ps[i];
        s0 = mstv->s[i];
    }

    // Array of UH1 values
    double *uh1;
    unsigned int uh1_l;
    uh1_f(modp->x4, &uh1, &uh1_l);
    /* print_uh(uh1, uh1_l); */

    // Array of UH2 values
    double *uh2;
    unsigned int uh2_l;
    uh2_f(modp->x4, &uh2, &uh2_l);
    /* print_uh(uh2, uh2_l); */

    // ROUTING
    double r0 = 0.1*modp->x3; // Initializing the routing store level
    unsigned int t;
    for(i = 0; i < ntimes; i++){ // Temporal loop
        mfxv->qb[i] = 0.0;
        mfxv->qa[i] = 0.0;
        if (i + 1 >= uh2_l)
        {
            for (t = 0; t < uh1_l; t++)
            {
                mfxv->qa[i] += *(uh1 + t) * mfxv->pr[i-t] * PA; 
            }
            for (t = 0; t < uh2_l; t++)
            {
                mfxv->qb[i] += *(uh2 + t) * mfxv->pr[i-t] * PB;
            }
        } else if (i + 1 >= uh1_l && i + 1 < uh2_l)
        {
            for (t = 0; t < uh1_l; t++)
            {
                mfxv->qa[i] += *(uh1 + t) * mfxv->pr[i-t] * PA; 
            }
            for (t = 0; t < i + 1; t++)
            {
                mfxv->qb[i] += *(uh2 + t) * mfxv->pr[i-t] * PB;
            }
        } else if (i + 1 < uh1_l)
        {
            for (t = 0; t < i + 1; t++)
            {
                mfxv->qa[i] += *(uh1 + t) * mfxv->pr[i-t] * PA; 
            }
            for (t = 0; t < i + 1; t++)
            {
                mfxv->qb[i] += *(uh2 + t) * mfxv->pr[i-t] * PB;
            }
        }
       
        // Catchment water exchage
        mfxv->f[i] = water_exch(modp->x2, modp->x3, r0);

        // Inflow to the routing store
        /* qa = Q_f(pr, uh1, uh1_l, PA);    */
        
        // Update the routing store water level
        mstv->r[i] = R_f1(r0, mfxv->qa[i], mfxv->f[i]);
        
        // Outflow from the routing store
        mfxv->qr[i] = Qr_f(mstv->r[i], modp->x3);
        
        // Final update the routing store water level
        mstv->r[i] = mstv->r[i] - mfxv->qr[i];
        /* Note that r must be less or equal to modp->x3 */
        if (mstv->r[i] > modp->x3){
            mstv->r[i] = modp->x3;
        }
       
        // Direct outflow
        /* qb = Q_f(pr, uh2, uh2_l, PB);    */
        mfxv->qd[i] = Qd_f(mfxv->qb[i], mfxv->f[i]);
        
        // Total streamflow
        /* runoff_s[i] = (qr + qd)*dummy; // Discharge in m^3/s */
        mfxv->q[i] = (mfxv->qr[i] + mfxv->qd[i]); // Discharge in m^3/s
        /* printf("The streamflow is:%f\n",runoff_s[i]); */

        r0 = mstv->r[i]; // Updating the past routing store level
    }
    free(uh1);
    free(uh2);
    /* return 0; */
}

#elif MODEL == 2 // HBV

    /* void hbv(const modparam *modp, const unsigned int ntimes, modstvar *mostv){ */
    void hbv(const unsigned int ntimes, const double *tav, const double *precip,  const double *et, const modparam *modp, modstatev *mstv, modfluxv *mfxv){

       unsigned int i;

       /* Main computation */
       double efprecip = 0.0;
       double Qall = 0.0;
       for(i = 1; i < ntimes; i++){ // Temporal loop

            /* Computation of effective precipitation (snow model) */
            efprecip = snowModel(i, tav[i], precip[i], mstv->sdep, modp->ttlim, modp->degw, modp->degd);
            /* printf("Effective precipitation =  %lf, sdep = %lf\n", efprecip, mstv->sdep[i]); */

            /* Computation of the shallow soil storage (soil model) */
            soilModel(efprecip, i, modp->fcap,  modp->lp,  modp->beta, et[i], mstv->sowat, mstv->stw1, mfxv->acet);
            /* printf("fcap = %lf, lp = %lf, beta = %lf, tave = %lf, et = %lf, sowat =  %lf, stw1 = %.10e, acet = %lf\n",modp->fcap, modp->lp, modp->beta, tav[i], et[i], mstv->sowat[i], mstv->stw1[i], mfxv->acet[i]); */

            /* Computation of discharge (discharge model) */
            Qall = dischargeModel(i, mstv->stw1, mstv->stw2, modp->hl1, modp->k0, modp->k1, modp->k2, modp->perc);
            /* printf("Qall = %.8e\n",Qall); */
            /* printf("hl1 =  %lf, k0 = %lf, k1 = %lf, k2 = %lf, perc = %lf\n", modp->hl1, modp->k0, modp->k1, modp->k2, modp->perc); */

            /* Computation of runoff routing (routing model) */
            routingModel(Qall, i, modp->maxbas, mfxv->qrou, mfxv->qsim);
            /* printf("maxbas = %d, qsim = %.8e\n", modp->maxbas,  mfxv->qsim[i]); */

            /* Reinitilize to zero routing arrays based on maxbas */
            backflowModel(modp->maxbas, mfxv->qrou);
            /* printf("qrou =  %lf\n", mfxv->qrou[i]); */

        }

       
    }


#elif MODEL == 3 // HYMOD

    void hymod(const unsigned int ntimes, const double *tav, const double *precip,  const double *et, const modparam *modp, modstatev *mstv, modfluxv *mfxv){

       unsigned int i, m;
       /* k = 0; */
       double new_quickflow, new_slowflow;
       double totalFlow = 0;
       for(i = 0; i < ntimes; i++){ // Temporal loop

            /* Computation of effective precipitation (snow model) */
            /* mfxv->efprecip[i] = snowDD(i, dataDay); */
            /* efprecip = snowModel(i, tav[i], precip[i], mstv->sdep, modp->ttlim, modp->degw, modp->degd); */
            mfxv->efprecip[i] = snowModel(i, tav[i],  precip[i], mfxv->snow, mstv->snow_store, mfxv->melt, modp->tth, modp->tb, modp->ddf);

            /* Run Pdm soil moisture accounting  */
            soilModel(mfxv->efprecip[i], i, modp->cpar, modp->huz, modp->b, et[i], modp->kv, mstv->xhuz, mfxv->ov, mfxv->ae, mstv->xcuz);
            
            // Run Nash Cascade routing of quickflow component
            new_quickflow = modp->alpha * mfxv->ov[i];
            mfxv->qq[i] = Nash(modp->kq, modp->nq, new_quickflow, mstv->xq[i]);

            // Run Nash Cascade routing of slowflow component
            new_slowflow = (1.0-modp->alpha) * mfxv->ov[i];
            mfxv->qs[i] = Nash(modp->ks, 1, new_slowflow, &mstv->xs[i]);

            // Set the intial states of the next time step to those of the current time step
            if (i < ntimes-1){
                mstv->xhuz[i+1] = mstv->xhuz[i];
                mstv->xs[i+1]   = mstv->xs[i];
                mstv->snow_store[i+1] = mstv->snow_store[i];

                for(m = 0; m < modp->nq; m++){
                    mstv->xq[i+1][m] = mstv->xq[i][m];
                    /* mstv->xq[i+1+m+modp->nq] = mstv->xq[i+m]; */
                    /* mstv->xq[k+modp->nq] = mstv->xq[k]; */
                    /* k++; */
                }
            }

            mfxv->q[i] = mfxv->qq[i] + mfxv->qs[i];
            totalFlow += mfxv->q[i]; 
        }
       printf("Total flow = %lf mm \n", totalFlow);
    }



#else // IAHCRES


#endif




