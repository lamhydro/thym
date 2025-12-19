/*
 * =====================================================================================
 *
 *       Filename:  writeout.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/10/25 21:07:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "writeout.h"

/*
 * Allocate memory for the modstatev (model-state variables) struct
 */
int allocateMemo_modstatev(const unsigned int size, modstatev *mstv){
    #if MODEL == 1 // GR4J
  
    #elif MODEL == 2 // HBV
        
        mstv->sowat = (double *)calloc(size, sizeof(double));
        mstv->sdep = (double *)calloc(size, sizeof(double));
        mstv->ldep = (double *)calloc(size, sizeof(double));
        mstv->stw1 = (double *)calloc(size, sizeof(double));
        mstv->stw2 = (double *)calloc(size, sizeof(double));
        // Check if malloc succeeded
        if (!mstv->sowat || !mstv->sdep || !mstv->ldep || !mstv->stw1 || !mstv->stw2) {
            perror("malloc failed");
            free(mstv->sowat);
            free(mstv->sdep);
            free(mstv->ldep);
            free(mstv->stw1);
            free(mstv->stw2);
            return 1;
        }

    #elif MODEL == 3 // HYMOD
    
    
    #else // IAHCRES
    
    
    #endif

    return 0;
}

/*
 * Allocate memory for the modfluxv (model-fluxes variables) struct
 */
int allocateMemo_modfluxv(const unsigned int size, modfluxv *mfxv){
    #if MODEL == 1 // GR4J
  
    #elif MODEL == 2 // HBV
       
        mfxv->qrou = (double *)calloc(size, sizeof(double));
        mfxv->qsim = (double *)calloc(size, sizeof(double));
        mfxv->acet = (double *)calloc(size, sizeof(double));
        // Check if malloc succeeded
        if (!mfxv->qrou || !mfxv->qsim || !mfxv->acet) {
            perror("malloc failed");
            free(mfxv->qrou);
            free(mfxv->qsim);
            free(mfxv->acet);
            return 1;
        }

    #elif MODEL == 3 // HYMOD
    
    #else // IAHCRES
    
    #endif

    return 0;
}


/*
 * Free memory for the modstatev (model-state variables) struct
 */
void freeMemo_modstatev(modstatev *mstv){
    #if MODEL == 1 // GR4J
    
    #elif MODEL == 2 // HBV
 
        free(mstv->sowat);
        free(mstv->sdep);
        free(mstv->ldep);
        free(mstv->stw1);
        free(mstv->stw2);

    #elif MODEL == 3 // HYMOD
    
    
    #else // IAHCRES
    
    
    #endif
}

/*
 * Free memory for the modfluxv (model-fluxes variables) struct
 */
void freeMemo_modfluxv(modfluxv *mfxv){
    #if MODEL == 1 // GR4J
    
    #elif MODEL == 2 // HBV
 
        free(mfxv->qrou);
        free(mfxv->qsim);
        free(mfxv->acet);

    #elif MODEL == 3 // HYMOD
    
    
    #else // IAHCRES
    
    
    #endif
}



/*
 * Print main model results
 */
void save_model_results(char *filename, const unsigned int n, struct tm *timestamp, const double *et, const modstvar *mostv, const snowstvar *sstvar){

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    // Write header

    #if MODEL == 1 // GR4J
        #if SNOWM == 0 // NO SNOW CALCULATIONS
            fprintf(fp, "datetime,et_mm,pn_mm,ps_mm,pr_mm,en_mm,es_mm,s_mm,r_mm,perc_mm,f_mm,qa_mm,qb_mm,qr_mm,qd_mm,q_mm\n");

        #elif SNOWM == 1 // 
            fprintf(fp, "datetime,rainfall_mm,snowfall_mm,sno_mm,tsnow_oC,snomlt_mm,eres_mm,et_mm,pn_mm,ps_mm,pr_mm,en_mm,es_mm,s_mm,r_mm,perc_mm,f_mm,qa_mm,qb_mm,qr_mm,qd_mm,q_mm\n");
        #endif
    #elif MODEL == 2 // HBV
        #if SNOWM == 0 // NO SNOW CALCULATIONS
        #elif SNOWM == 1 // 
        #endif
   
    
    #elif MODEL == 3 // HYMOD
    
        #if SNOWM == 0 // NO SNOW CALCULATIONS
        #elif SNOWM == 1 // 
        #endif
    
    #else // IAHCRES
        #if SNOWM == 0 // NO SNOW CALCULATIONS
        #elif SNOWM == 1 // 
        #endif
    
    
    #endif




    unsigned int i;
    char buffer[30];
    for (i = 0; i < n; i++) {
        /* printf("year: %d, month: %d, day: %d, jday: %d\n", timestamp[i].tm_year, timestamp[i].tm_mon,timestamp[i].tm_mday, timestamp[i].tm_yday+1); */
        // Format timestamp as YYYY-MM-DD HH:MM:SS
        timestamp[i].tm_year -= 1900;
        timestamp[i].tm_mon -= 1;
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timestamp[i]);

        #if MODEL == 1 // GR4J
            #if SNOWM == 0 // NO SNOW CALCULATIONS
                fprintf(fp, "%s,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf\n", buffer, et[i], mostv[i].pn, mostv[i].ps, mostv[i].pr, mostv[i].en, mostv[i].es, mostv[i].s, mostv[i].r, mostv[i].perc, mostv[i].f, mostv[i].qa, mostv[i].qb, mostv[i].qr, mostv[i].qd, mostv[i].q);
            
            #elif SNOWM == 1 // 
                fprintf(fp, "%s,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf\n", buffer, sstvar->rainfall[i], sstvar->snowfall[i], sstvar->sno[i], sstvar->tsnow[i], sstvar->snomlt[i], sstvar->eres[i], et[i], mostv[i].pn, mostv[i].ps, mostv[i].pr, mostv[i].en, mostv[i].es, mostv[i].s, mostv[i].r, mostv[i].perc, mostv[i].f, mostv[i].qa, mostv[i].qb, mostv[i].qr, mostv[i].qd, mostv[i].q);
             #endif

        #elif MODEL == 2 // HBV
            #if SNOWM == 0 // NO SNOW CALCULATIONS
            #elif SNOWM == 1 // 
            #endif
   
        
        #elif MODEL == 3 // HYMOD
        
            #if SNOWM == 0 // NO SNOW CALCULATIONS
            #elif SNOWM == 1 // 
            #endif
        
        #else // IAHCRES
            #if SNOWM == 0 // NO SNOW CALCULATIONS
            #elif SNOWM == 1 // 
            #endif
        
        
        #endif


            }

    fclose(fp);
    /* printf("Time series written to %s\n", filename); */

}

/*
 * Print state and flux variables
 */
void save_model_results2(char *filename, const unsigned int n, struct tm *timestamp, const modstatev *mstv, const modfluxv *mfxv){

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }


    // Write header

    #if MODEL == 1 // GR4J

    #elif MODEL == 2 // HBV
        fprintf(fp, "datetime,sowat_mm,sdep_mm,ldep_mm,stw1_mm,stw2_mm,qsim_mm,acet_mm\n");
    #elif MODEL == 3 // HYMOD
    
        
    #endif


    unsigned int i;
    char buffer[30];
    for (i = 0; i < n; i++) {
        /* printf("year: %d, month: %d, day: %d, jday: %d\n", timestamp[i].tm_year, timestamp[i].tm_mon,timestamp[i].tm_mday, timestamp[i].tm_yday+1); */
        // Format timestamp as YYYY-MM-DD HH:MM:SS
        timestamp[i].tm_year -= 1900;
        timestamp[i].tm_mon -= 1;
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timestamp[i]);

        #if MODEL == 1 // GR4J
                       

        #elif MODEL == 2 // HBV
   
            fprintf(fp,"%s,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8e,%.8lf\n",buffer, mstv->sowat[i], mstv->sdep[i], mstv->ldep[i], mstv->stw1[i], mstv->stw2[i], mfxv->qsim[i], mfxv->acet[i]);
        
        #elif MODEL == 3 // HYMOD
        
        
        #else // IAHCRES
        
        
        #endif


            }

    fclose(fp);
    /* printf("Time series written to %s\n", filename); */

}
