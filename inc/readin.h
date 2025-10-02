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
    float *tave, *tmin, *tmax, *precip, *disch, *srad, *et;
} meteoin;

typedef struct {
    int ntimes, sdt, edt;
} meteoini;

typedef struct {
    struct tm startdt;
    struct tm enddt;
    float lat, lon, alt, area;
    int dt;
    char name[MAX_LINE_LENGTH];
} ctrlbasic;

typedef struct {
    int nvarout;
    char **varout;
    float dtout;
} ctrlout;

typedef struct{
    char etmethod[10];
    float *et;
} evapot;

typedef struct {
    char model[10];
    float *Q;

    #if MODEL == 1 // GR4J
    
        float x1, x2, x3;
        int x4;
    
    #elif MODEL == 2 // HBV
    
        float w1, w2, w3, w4;
    
    #elif MODEL == 3 // HYMOD
    
        float y1, y2, y3, y4;
    
    #else // IAHCRES
    
        float z1, z2, z3, z4;
    
    #endif
} modparam;


/*
 * Function prototypes
 */
int read_ctrl(char *ctrlf, ctrlbasic *ctrlb, ctrlout *ctrlo, modparam *modp, char *etmethod);
//int read_ctrl(char *ctrlf);
int get_time_interval_meteo_in(char *meteof, struct tm sdt, struct tm edt, meteoini *metinii);
int read_meteo(char *meteof, meteoin *metin, meteoini *metini, struct tm *dts, float *et); 
int read_init(char *testcase, ininfo *info);
void freeininfo(ininfo *info);
void freectrlout(ctrlout *ctrlo);
int allocateMemo(int size, meteoin *metin);
void save_model_results(char *filename, int n, struct tm *timestamp, float *et, float *disch);

#endif // End of include guard
