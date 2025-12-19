/*
 * =====================================================================================
 *
 *       Filename:  snow.c
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

#include "snow.h"
/*
 * Allocate memory fo the snowstvar struct
 */
int allocateMemoSnow(int size, snowstvar *sstvar){
    sstvar->rainfall = (double *)malloc(size * sizeof(double));
    sstvar->snowfall = (double *)malloc(size * sizeof(double));
    sstvar->sno = (double *)malloc(size * sizeof(double));
    sstvar->tsnow = (double *)malloc(size * sizeof(double));
    sstvar->snomlt= (double *)malloc(size * sizeof(double));
    sstvar->eres= (double *)malloc(size * sizeof(double));
    // Check if malloc succeeded
    if (!sstvar->rainfall || !sstvar->snowfall || !sstvar->sno || !sstvar->tsnow || !sstvar->snomlt || !sstvar->eres) {
        perror("malloc failed");
        free(sstvar->rainfall);
        free(sstvar->snowfall);
        free(sstvar->sno);
        free(sstvar->tsnow);
        free(sstvar->snomlt);
        free(sstvar->eres);
        return 1;
    }

    return 0;
}

/*
 * Free memory fo the snowstvar struct
 */
void freeMemoSnow(snowstvar *sstvar){
    free(sstvar->rainfall);
    free(sstvar->snowfall);
    free(sstvar->sno);
    free(sstvar->tsnow);
    free(sstvar->snomlt);
    free(sstvar->eres);
}
#if MODEL == 1 // GR4J
#if SNOWM == 0 // NO SNOW CALCULATIONS
    void rainOrSnow_f0(const unsigned int ntimes, const double *precip, double *rainfall, double *snowfall){
        unsigned int i;
        for (i = 0; i < ntimes; i++)
            {
                /* (p_var_snow + i)->Rainfall = (p_vars_in + i)->Prec; */
                rainfall[i] = precip[i];
                /* (p_var_snow + i)->Snowfall = 0.0; */
                snowfall[i] = 0.0;
            }        
    }               
#elif SNOWM == 1 // 

    /******************
     * parameters of the degree-day snow amodule
     * 
      # Tmlt        the base temperature above which snow melt is allowed
      # SNO50       the depth of snow when the basin is 50% covered by snow, unit:mm
      # SNO100      the shreshold depth of snow, above which there is 100% cover  , unit:mm
      # Ls          snow temperature lag factor
      # Trs         air temperature above which the precipitation is all rainfall, 
      # Bmlt6       melt factor on June 21 (mm H2O/day-℃)
      # Bmlt12      melt factor on December 21 (mm H2O/day-℃)
      # Tsnow_0     initial snow temperature
      # SNO_0       initial snow depth
     * **********/
    void rainOrSnow_f1(const unsigned int ntimes, const double *precip, double trs, double *tav, double *rainfall, double *snowfall){

        unsigned int i;
        for (i = 0; i < ntimes; i++){
            if (tav[i] >= trs)
            {
                rainfall[i] = precip[i];
                snowfall[i] = 0.0;
            } else {
                snowfall[i] = precip[i];
                rainfall[i] = 0.0;
            }
        }
    }
    
    /**************************
     * variables:
     * int dn               number of day in a year
     * double Tav       	daily average air temperature
     * double Tmax          daily maximum air temperature
     * double Ep            daily potential evapotranspiration
     * double SNO0          snow for the previous step
     * double Tsnow0        snow temperature for the previous step
     * double *SNO          snow after this step
     * double *Tsnow        snow temperature after this step
     * double *SNOmlt       snowmelt for this time step
     * double *Eres         residual evapotranspiration
     * 
     * **********************/
    /* void snowModel(const int ntimes, int dn, double snowfall, double tav, double tmax, double ep, double tsnow0, double sno0, double *tsnow, double *sno, double *snomlt, double *eres, double sno50, double sno100, double ls, double bmlt6, double bmlt12, double tmlt){ */
/* Model_SNOW( */
            /* (ts_date + i)->dn, */
            /* (p_var_snow + i)->Snowfall, */
            /* (p_vars_in + i)->Tair, */
            /* (p_vars_in + i)->Tmax, */
            /* (p_vars_in + i)->Epot, */
            /* Tsnow0, */
            /* SNO0, */
            /* &((p_var_snow + i)->Tsnow), */
            /* &((p_var_snow + i)->SNO), */
            /* &((p_var_snow + i)->SNOmlt), */
            /* &((p_var_snow + i)->Eres), */
            /* Para_snow.SNO50, */
            /* Para_snow.SNO100, */
            /* Para_snow.Ls, */
            /* Para_snow.Bmlt6, */
            /* Para_snow.Bmlt12, */
            /* Para_snow.Tmlt);     */

    void snowModel(const unsigned int ntimes, struct tm *timestamp, double *snowfall, double *tav, double *tmax, double *ep, double *tsnow, double *sno, double *snomlt, double *eres, snowparam *snowp){
    
        unsigned int i;
        double sno0 = 5;
        double tsnow0 = -5;
        double ratio;
        double cov1, cov2;
        double sno_cov;
        double bmlt;
        int dn;
        ratio = snowp->sno50 / snowp->sno100;
        cov2 = log(0.05 / ratio) / (ratio - 0.95);
        cov1 = log(0.05) + 0.95 * cov2;
        for (i = 0; i < ntimes; i++){
            dn = timestamp[i].tm_yday+1;// +1 because the counter of yday start at 0. */
            // if (SNO50 <= 0.0)
            // {
            //     printf("Error: SNO50 cannot be zero.\n");
            //     return -1; // Indicate an error
            // }
            // if (ratio <= 0.0)
            // {
            //     printf("Error: SNO50 / SNO100 must be positive.\n");
            //     return -1; // Indicate an error
            // }
            
            sno[i] = sno0 + snowfall[i];
            if (sno[i] >= ep[i])
            {
                sno[i] = sno[i] - ep[i];
                eres[i] = 0.0;
            }
            else
            {
                eres[i] = ep[i] - sno[i];
                sno[i] = 0.0;
            }
            if (sno[i] <= 0.0001)
            {
                snomlt[i] = 0.0;
                // *SNO = 0.0;
                tsnow[i] = tav[i];
            }
            else
            {
                sno_cov = (sno[i] / snowp->sno100) / (sno[i] / snowp->sno100 + exp(cov1 - cov2 * sno[i] / snowp->sno100));
                tsnow[i] = tsnow0 * (1 - snowp->ls) + tav[i] * snowp->ls;
                bmlt = (snowp->bmlt6 + snowp->bmlt12) / 2 + (snowp->bmlt6 - snowp->bmlt12) / 2 * sin(2 * M_PI / 365 * (dn - 81));
                snomlt[i] = bmlt * sno_cov * (tsnow[i] / 2 + tmax[i] / 2 - snowp->tmlt);
                if (snomlt[i] < 0.001)
                {
                    snomlt[i] = 0.0;
                }
                
                sno[i] = sno[i] - snomlt[i];
                if (sno[i] < 0.001)
                {
                    sno[i] = 0.0;
                    tsnow[i] = tav[i];
                }
            }
            sno0 = sno[i];
            tsnow0 = tsnow[i];
        }
    }
    
              
#endif
             
#elif MODEL == 2 // HBV
                 
    /* Define weather precipitation is either snowfall or rainfall
     * 
    * tav: Average temperature [oC].
    * ttlim: Temperature threshold below which freezing occurs [oC]
    * precip: Precipitation in a day [mm]
    * raifall: Liquid precipitation in a day [mm]
    * snowfall: Snowfall in a day [mm]
    */
    /* void rainOrSnow_hbv(const double tav, const double ttlim, const double precip, double *rainfall, double *snowfall){ */
        /* rainfall = precip; */
        /* snowfall = 0.0; */
        /* if (tav < ttlim){ */
            /* rainfall = 0.0; */
            /* snowfall = precip; */
        /* } */
    /* } */

    /* Compute snowmelt
     *
    * tav: Average temperature [oC].
    * degw: Base temperature above which melt occurs [oC].
    * degd: Degree day factor (snowmelt rate) [mm/(day oC)]
    */
    /* double snowmelt(const double tav, const double degw, const double degd){ */
        /* double smelt = 0.0; */
        /* if (tav > degw){ */
            /* smelt = (tav-degw)*degd; */
        /* }  */
        /* return smelt; */
    /* } */

    /* Define weather precipitation is either snowfall or rainfall and compute snowmelt
     * 
     * ntimes: Number of time steps
     * tav: Average temperature [oC].
     * ttlim: Temperature threshold below which freezing occurs [oC]
     * precip: Precipitation in a day [mm]
     * raifall: Liquid precipitation in a day [mm]
     * snowfall: Snowfall in a day [mm]
     * degw: Base temperature above which melt occurs [oC].
     * degd: Degree day factor (snowmelt rate) [mm/(day oC)]
     * snomlt: snowmelt rate [mm/day]
    */
/*     void snowModel(const unsigned int ntimes, const double *tav, const double degw, const double degd, const double ttlim, const double *precip, double *rainfall, double *snowfall, double *snomlt){ */

        /* unsigned int i; */
        /* for (i = 0; i < ntimes; i++){ */
            
            /* [> Definition whether precipitation is either snowfall or rainfall<] */
            /* rainfall[i] = precip[i]; */
            /* snowfall[i] = 0.0; */
            /* if (tav[i] < ttlim){ */
                /* rainfall[i] = 0.0; */
                /* snowfall[i] = precip[i]; */
            /* } */

            /* [> Compute snowmelt <] */
            /* snomlt[i] = 0.0; */
            /* if (tav[i] > degw){ */
                /* snomlt[i] = (tav[i]-degw)*degd; */
            /* } */
 
        /* } */
    /* } */

/* Estimate effective precipitation */
double snowModel(const unsigned int i, const double tav, const double precip, double *sdep, const double ttlim, const double degw, const double degd)
{
    /* printf("i = %d, tav = %lf, precip = %lf, ttlim = %lf, degw = %lf, degd = %lf\n", i, tav, precip, ttlim, degw, degd); */

    double snomlt = 0.0;
    double efprecip = 0.0; //effective precip initialized to zero

    // starting point: equal to yesterday
    sdep[i] = sdep[i-1];

    // Snow/Rain
    if (tav < ttlim)
        sdep[i] += precip;    // if temperature is lower than threshold (ttlim) --> precip is all snow
	else 
        efprecip += precip;               // otherwise --> add precip to effective precip

    // Snow melt if temperature > threshold (degw)
    if (tav > degw)
    {
        //If there is actually snow to melt in the snow store...
        if (sdep[i] > 0.0)
        {
            //Calculate snow melt using degree-day factor (degd)
            snomlt = (tav - degw)*degd;
            //If snow melt that wants to occur is more than what is actually stored...
            if (snomlt > sdep[i])
            {
                efprecip += sdep[i];    //add full snow depth to effective precip
                sdep[i] = 0.0;            //All of the snow has melted
            }
            else //Otherwise, we melt a portion of the snow store
            {
                efprecip += snomlt;                //effective precip is precip together with what acutally melted
                sdep[i] -= snomlt;     //Remove the amount that melted from the snow store
            }
        }
    }

    return efprecip;
}

#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif



