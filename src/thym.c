#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "snow.h"
#include "readin.h"
#include "writeout.h"
#include "evapotrans.h"
#include "model.h"


int main(int argc, char **argv){
    // argc: Number of command-line arguments
    // argv: Array of command-line arguments

    if (argc < 2) {
        printf("Usage: %s <arguments>\n", argv[0]);
        return 1;
    }

    //--------- Print banner with basic information
    print_box_banner("Tropical Hydrological Modeling (THYM)", "Author: Luis Morales-Marin", "Universidad Nacional de Colombia");

    //--------- Read test-case name
    //read_init(argv[1]);
    ininfo ininf;
    read_init(argv[1], &ininf);
    /* printf("%s\n", ininf.meteoinf); */

    //--------- Read ctrl.in file
    ctrlbasic ctrlb;
    ctrlout ctrlo;
    modparam modp;
    evapot evp;

    snowparam snowp;
    #if MODEL == 1 // GR4J
    #elif MODEL == 2 // HBV
    #elif MODEL == 3 // HYMOD
    #endif

    read_ctrl(ininf.ctrlinf, &ctrlb, &ctrlo, &modp, evp.etmethod, &snowp);
    //read_ctrl(ininf.ctrlinf);

    //--------- Read meteo.in file
    // Get the time interval to read in meteo.in input file
    meteoini metini;
    get_time_interval_meteo_in(ininf.meteoinf, ctrlb.startdt, ctrlb.enddt, &metini);
    printf("sdt: %d, edt: %d, ntimes: %d \n", metini.sdt, metini.edt, metini.ntimes); 
    
    // Read the lengh to the time series
    /* int nlines = get_file_nlines(ininf.meteoinf, NHEADER_LINES_METEOIN);  */
    //printf("The number of lines in meteo.in: %d\n",nlines);
    //meteoin metin[nlines];
    /* meteoin *metin = (meteoin *)malloc(metini.ntimes * sizeof(meteoin)); */
    meteoin metin;

    allocateMemo(metini.ntimes, &metin);
/*     metin.timestamp = (struct tm *)malloc(metini.ntimes * sizeof(struct tm)); */
    /* metin.tave = (float *)malloc(metini.ntimes * sizeof(float)); */
    /* metin.tmin = (float *)malloc(metini.ntimes * sizeof(float)); */
    /* metin.tmax = (float *)malloc(metini.ntimes * sizeof(float)); */
    /* metin.precip = (float *)malloc(metini.ntimes * sizeof(float)); */
    /* metin.disch = (float *)malloc(metini.ntimes * sizeof(float)); */
    /* [> metin.srad = malloc(metini.ntimes * sizeof(float)); <] */
    /* // Check if malloc succeeded */
    /* if (!metin.timestamp || !metin.tave || !metin.tmin || !metin.tmax || !metin.precip || !metin.disch) { */
        /* perror("malloc failed"); */
        /* free(metin.timestamp); */
        /* free(metin.tave); */
        /* free(metin.tmin); */
        /* free(metin.tmax); */
        /* free(metin.precip); */
        /* free(metin.disch); */
        /* [> free(metin.srad); <] */
        /* return 1; */
    /* } */


    /* double *tave, *tmin, *tmax, *precip, *disch, *srad, *et; */
    // Check if malloc succeeded
/*     if (metin == NULL) { */
        /* printf("Memory allocation failed!\n"); */
        /* return 1; */
    /* } */
    

    struct tm *dts = (struct tm *)malloc(metini.ntimes * sizeof(struct tm));
    // Check if malloc succeeded
    if (dts == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    //struct tm dts[nlines];
    //read_meteo("../test/meteo.in");
    evp.et = (double *)malloc(metini.ntimes * sizeof(double));
    // Check if malloc succeeded
    if (evp.et == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    read_meteo(ininf.meteoinf, &metin, &metini, dts, evp.et);
    printf("year: %d, month: %d, day: %d, jday: %d, et: %f, runoff: %f\n", metin.timestamp[0].tm_year, metin.timestamp[0].tm_mon, metin.timestamp[0].tm_mday, metin.timestamp[0].tm_yday+1, evp.et[0], metin.runoff[0]);
    printf("year: %d, month: %d, day: %d, jday: %d, et: %f\n", metin.timestamp[metini.ntimes-1].tm_year, metin.timestamp[metini.ntimes-1].tm_mon,metin.timestamp[metini.ntimes-1].tm_mday, metin.timestamp[metini.ntimes-1].tm_yday+1, evp.et[metini.ntimes-1]);

    
    //--------- Compute evapotranspiration
    /* ets(&evp, metin, ctrlb.lat, metini.ntimes); */
    ets(evp.etmethod, metin.tave, metin.timestamp, ctrlb.lat, metini.ntimes, evp.et);
    /* for(int i=0; i<metini.ntimes; i++){ */
        /* [> printf("et[%d] = %f, precip[%d] = %f\n", i, evp.et[i], i, metin.precip[i]); <] */
    /* } */

    //--------- Set up the simulation start and end datetime indexes
    // Read start datetime in meteoin
    /* printf("No. lines is: %d\n", nlines); */
    /* int startdti = get_time_index(nlines, dts, ctrlb.startdt); */
    /* printf("simulation start datetime index:%d\n", startdti); */
        
    // Read start datetime in meteoin
    /* int enddti = get_time_index(nlines, dts, ctrlb.enddt); */
    /* printf("simulation end datetime index:%d\n", enddti); */

	printf("\n");
	printf("******* RUNNING '%s' FOR TESTCASE '%s' *******\n",modp.model, argv[1]);
	printf("\n");

    /* modp.runoff = (double *)malloc(metini.ntimes * sizeof(double)); */
    /* // Check if malloc succeeded */
    /* if (modp.runoff == NULL) { */
        /* printf("Memory allocation failed!\n"); */
        /* return 1; */
    /* } */

    // Model implementation
    modstvar *mostv = (modstvar *)malloc(metini.ntimes * sizeof(modstvar));
    snowstvar sstvar;
    allocateMemoSnow(metini.ntimes, &sstvar);
    modstatev mstv;
    allocateMemo_modstatev(metini.ntimes, &mstv);
    modfluxv mfxv;
    allocateMemo_modfluxv(metini.ntimes, &mfxv);
    #if MODEL == 1 // GR4J
                   
        //--------- Computation for snow processes 
        #if SNOWM == 0 // 
            rainOrSnow_f0(metini.ntimes, metin.precip, sstvar.rainfall, sstvar.snowfall);
        #elif SNOWM == 1 //
            printf("\n");
	        printf("******* RUNNING SNOW MODEL '%s' FOR TESTCASE '%s' *******\n",modp.model, argv[1]);
	        printf("\n");
            rainOrSnow_f1(metini.ntimes, metin.precip, snowp.trs, metin.tave, sstvar.rainfall, sstvar.snowfall);
            snowModel(metini.ntimes, metin.timestamp, sstvar.snowfall, metin.tave, metin.tmax, evp.et, sstvar.tsnow, sstvar.sno, sstvar.snomlt, sstvar.eres, &snowp);
        #endif


         //gr4j(metin, modp);
        /* gr4j(metin.precip, evp.et, &modp, metini.ntimes, mostv); */
        //--------- Rainfall-runoff modelling
        gr4j(sstvar.rainfall, sstvar.snomlt, sstvar.eres, &modp, metini.ntimes, mostv);
    
    #elif MODEL == 2 // HBV
/*         //--------- Computation for snow processes  */
        /* #if SNOWM == 0 //  */
        /* #elif SNOWM == 1 // */
            /* printf("\n"); */
			/* printf("******* RUNNING SNOW MODEL '%s' FOR TESTCASE '%s' *******\n",modp.model, argv[1]); */
			/* printf("\n"); */
            /* snowModel(metini.ntimes, metin.tave, modp.degw, modp.degd, modp.ttlim, metin.precip, sstvar.rainfall, sstvar.snowfall, sstvar.snomlt); */
        /* #endif */
 
        //--------- Rainfall-runoff modelling
        hbv(metini.ntimes, metin.tave, metin.precip, evp.et, &modp, &mstv, &mfxv);
    
    
    #elif MODEL == 3 // HYMOD
        //--------- Computation for snow processes 
        #if SNOWM == 0 // 
        #elif SNOWM == 1 //
            printf("\n");
	        printf("******* RUNNING SNOW MODEL '%s' FOR TESTCASE '%s' *******\n",modp.model, argv[1]);
	        printf("\n");
        #endif
 
        //--------- Rainfall-runoff modelling
    

    
    #else // IAHCRES
    
    
    #endif
   
    // Print model output
    #if MODEL == 1 // GR4J
        save_model_results(ininf.resultsoutf, metini.ntimes, metin.timestamp, evp.et, mostv, &sstvar);
    #else
        save_model_results2(ininf.resultsoutf, metini.ntimes, metin.timestamp, &mstv, &mfxv);
    #endif


    // Free memory 
    freeininfo(&ininf);  // from ininf
    freectrlout(&ctrlo); // from ctrlo 
    /* free(metin);         // from meteoin structure */
/*     free(metin.timestamp); */
    /* free(metin.tave); */
    /* free(metin.tmin); */
    /* free(metin.tmax); */
    /* free(metin.precip); */
    /* free(metin.disch); */
/*     / *[> free(metin.srad); */
    free(dts);           // from tm structure
    free(evp.et);           // 
    /* free(modp.runoff);           //  */
    free(mostv);           // 
    //free(ininf->tcase);
    //free(ininf->tcase);

    /* freememo(struct tm v1, float *v2, float *v3, float *v4, float *v5, float *v6); */
    freememo(metin.timestamp, metin.tave, metin.tmin, metin.tmax, metin.precip, metin.runoff);
    freeMemoSnow(&sstvar);
    freeMemo_modstatev(&mstv);
    freeMemo_modfluxv(&mfxv);
    #if MODEL == 1 // GR4J
    #elif MODEL == 2 // HBV
    
    
    #elif MODEL == 3 // HYMOD
    
    
    #else // IAHCRES
    
    
    #endif

    return 0;
}

