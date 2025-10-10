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

#include "utils.h"
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
    int ntimes, sdt, edt;
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

typedef struct {
    char model[10];

    #if MODEL == 1 // GR4J
    
        double x1, x2, x3, x4;
    
    #elif MODEL == 2 // HBV
    
        double w1, w2, w3, w4;
    
    #elif MODEL == 3 // HYMOD
    
        double y1, y2, y3, y4;
    
    #else // IAHCRES
    
        double z1, z2, z3, z4;
    
    #endif
} modparam;


/*
 * Function prototypes
 */
int read_ctrl(char *ctrlf, ctrlbasic *ctrlb, ctrlout *ctrlo, modparam *modp, char *etmethod);
//int read_ctrl(char *ctrlf);
int get_time_interval_meteo_in(char *meteof, struct tm sdt, struct tm edt, meteoini *metinii);
int read_meteo(char *meteof, meteoin *metin, meteoini *metini, struct tm *dts, double *et); 
int read_init(char *testcase, ininfo *info);
void freeininfo(ininfo *info);
void freectrlout(ctrlout *ctrlo);
int allocateMemo(int size, meteoin *metin);

#endif // End of include guard
