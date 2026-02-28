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

#if MODEL == 1 // GR4J

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

    void snowModel(const unsigned int i, struct tm *timestamp, double *snowfall, const double *tav, const double *tmax, const double *ep, double *tsnow, double *sno, double *snomlt, double *eres, const modparam *modp,  double cov1, double cov2, double *sno0, double *tsnow0){
    
        int dn = timestamp[i].tm_yday+1;// +1 because the counter of yday start at 0. */
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
        
        sno[i] = (*sno0) + snowfall[i];
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
            double sno_cov = (sno[i] / modp->sno100) / (sno[i] / modp->sno100 + exp(cov1 - cov2 * sno[i] / modp->sno100));
            tsnow[i] = (*tsnow0) * (1 - modp->ls) + tav[i] * modp->ls;
            double bmlt = (modp->bmlt6 + modp->bmlt12) / 2 + (modp->bmlt6 - modp->bmlt12) / 2 * sin(2 * M_PI / 365 * (dn - 81));
            snomlt[i] = bmlt * sno_cov * (tsnow[i] / 2 + tmax[i] / 2 - modp->tmlt);
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
        *sno0 = sno[i];
        *tsnow0 = tsnow[i];
    }
    
            
#elif MODEL == 2 // HBV
                 
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

    double snowModel(const unsigned int i, const double tav, const double precip, double *snow, double *snow_store, double *melt, const double tth, const double tb, const double ddf)
    {
        double Qout; // effective precip after freezing/melting
    
        //If temperature is lower than threshold, precip is all snow
        if (tav < tth)
        {
            snow[i] = precip;
            Qout = 0.0;
        }
        else //Otherwise, there is no snow and it's all rain
        {
            snow[i] = 0.0;
            Qout = precip;
        }
    
        //Add to the snow storage for this day
        snow_store[i] += snow[i];
    
        //Snow melt occurs if we are above the base temperature (either a fraction of the store, or the whole thing)
        if (tav > tb)
        {
            melt[i] = fmin(ddf*(tav-tb), snow_store[i]);
        }
        //Otherwise, snowmelt is zero
        else
        {
            melt[i] = 0.0;
        }
    
        //Update the snow storage depending on melt
        snow_store[i] -= melt[i];
        if(snow_store[i] < 0.0) snow_store[i] = 0.0;
    
        //Qout is any rain + snow melt
        Qout += melt[i];
    
        return Qout;
    }

#else // IAHCRES


#endif



