/*
 * =====================================================================================
 *
 *       Filename:  readin.h
 *
 *    Description:  Header file of readin.c 
 *
 *        Version:  1.0
 *        Created:  12/09/24 22:48:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (Luis Morales), 
 *   Organization:  UNAL
 *
 * =====================================================================================
 */

#ifndef READIN_H  // Check if MY_HEADER_H is not defined
#define READIN_H  // Define MY_HEADER_H
                      
#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "utils.h"
// #include "snow.h"
// #include "evapotrans.h"

/*
 * Defining constants
 */
#define MAX_LINE_LENGTH 1024
#define METEOIN_FILE "meteo.in"
#define CTRLIN_FILE "ctrl.in"
#define RESULTSOUT_FILE "results.out"
#define NHEADER_LINES_METEOIN 1

/*
 * Defining object types
 */
typedef struct {
    char *tcase, *ctrlinf, *meteoinf, *resultsoutf;
} ininfo;

typedef struct {
    struct tm *timestamp;
    double *tave, *tmin, *tmax, *precip, *runoff, *srad;
} meteoin;

typedef struct {
    unsigned int ntimes, sdt, edt;
} meteoini;

typedef struct {
    struct tm startdt;
    struct tm enddt;
    double lat, lon, alt, area;
    int dt;
    char name[MAX_LINE_LENGTH];
} ctrlbasic;

typedef struct {
    int nvarout;
    char **varout;
    int dtout;
} ctrlout;

typedef struct{
    char etmethod[10];
    double *et;
} evapot;

/* typedef struct{ */
    /* #if MODEL == 1 // GR4J */
        /* #if SNOWM == 0 //  */
        /* #elif SNOWM == 1 //  */
            /* double trs; */
            /* double tmlt; */
            /* double sno50; */
            /* double sno100; */
            /* double ls; */
            /* double bmlt6; */
            /* double bmlt12; */
        /* #endif */
    /* #elif MODEL == 2 // HBV */
    
    
    /* #elif MODEL == 3 // HYMOD */
    
    
    /* #else // IAHCRES */
    
    
    /* #endif */

/* } snowparam; */


typedef struct {
    char model[10];

    #if MODEL == 1 // GR4J
    
        double x1;          /* Maximum capacity of the production store [mm] */
        double x2;          /* Groundwater recharge coefficient */
        double x3;          /* One-day ahead maximun capacity of the routing store [mm] */
        double x4;          /* Time base of unit hydrograph UH1 */
        double trs;         /* Air temperature above which the precipitation is all rainfall [oC] */
        double tmlt;        /* The base temperature above which snow melt is allowed [oC] */
        double sno50;       /* The depth of snow when the basin is 50% covered by snow [mm] */
        double sno100;      /* The shreshold depth of snow, above which there is 100% cover [mm] */
        double ls;          /* Snow temperature lag factor */
        double bmlt6;       /* Melt factor on June 21 [mm H2O/day-oC] */
        double bmlt12;      /* Melt factor on December 21 [mm H2O/day-oC] */
        unsigned int nq;    /* Not used in GR4J */
 
    
    #elif MODEL == 2 // HBV
    
        double k2; 
        double k1; 
        double k0; 
        double degd;
        double degw; 
        double ttlim; 
        double perc; 
        double beta; 
        double lp; 
        double fcap;
        double hl1;
        int maxbas;
        unsigned int nq;         // Not used in HBV
    
    #elif MODEL == 3 // HYMOD

        //User specified parameters
        double huz;      //Maximum height of soil moisture accounting tank - Range [0, Inf]
        double b;        //Scaled distribution function shape parameter    - Range [0, 2]
        double alpha;    //Quick/slow split parameter                      - Range [0, 1]
        unsigned int    nq;       //Number of quickflow routing tanks               - Range [1, Inf] (typically set to <3)
        double kq;       //Quickflow routing tanks' rate parameter         - Range [0, 1]
        double ks;       //Slowflow routing tank's rate parameter          - Range [0, 1]

        // Snow parameters (degree-day model)
        double ddf;        //Degree day factor                        - Range [ 0, 2]
        double tth;        //Temperature threshold                    - Range [-5, 5] 
        double tb;         //Base temperature to calculate melt       - Range [-5, 5]

        // Given/calculated parameters
        double kv;       //Vegetation adjustment to PE                     - Range [0, 2]
        double cpar;     //Maximum combined contents of all stores (calculated from Huz and b)
    
    #else // IAHCRES
    
        double z1, z2, z3, z4;
    
    #endif
} modparam;


/*
 * Function prototypes
 */
// int read_ctrl(char *ctrlf, ctrlbasic *ctrlb, ctrlout *ctrlo, modparam *modp, char *etmethod, snowparam *snowp);
int read_ctrl(char *ctrlf, ctrlbasic *ctrlb, ctrlout *ctrlo, modparam *modp, char *etmethod);
//int read_ctrl(char *ctrlf);
int get_time_interval_meteo_in(char *meteof, struct tm sdt, struct tm edt, meteoini *metinii);
int read_meteo(char *meteof, meteoin *metin, meteoini *metini, struct tm *dts, double *et); 
int read_init(char *testcase, ininfo *info);
void freeininfo(ininfo *info);
void freectrlout(ctrlout *ctrlo);
int allocateMemo(int size, meteoin *metin);

#endif // End of include guard
