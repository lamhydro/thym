/*
 * =====================================================================================
 *
 *       Filename:  snow.h
 *
 *    Description:  Header file of channel.c 
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
#ifndef SNOW_H  // Check if MY_HEADER_H is not defined
#define SNOW_H  // Define MY_HEADER_H
                  
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "readin.h"
// #include "writeout.h"

// Snow state variables
typedef struct{
     double *rainfall;
     double *snowfall;
     double *sno;
     double *tsnow;
     double *snomlt;
     double *eres;
} snowstvar;

 
int allocateMemoSnow(int size, snowstvar *sstvar);
void freeMemoSnow(snowstvar *sstvar);
#if SNOWM == 0 // NO SNOW CALCULATIONS
    void rainOrSnow_f0(const int ntimes, const double *precip, double *rainfall, double *snowfall);
#elif SNOWM == 1 // 
  
    /*
     * Function prototypes
     */
    void rainOrSnow_f1(const int ntimes, const double *precip, double trs, double *tav, double *rainfall, double *snowfall);
    void snowModel(const int ntimes, struct tm *timestamp, double *snowfall, double *tav, double *tmax, double *ep, double *tsnow, double *sno, double *snomlt, double *eres, snowparam *snowp);
    // void snowModel(int dn, double snowfall, double tav, double tmax, double ep, double tsnow0, double sno0, double *tsnow, double *sno, double *snomlt, double *eres, double sno50, double sno100, double ls, double bmlt6, double bmlt12, double tmlt);
                
#endif


#endif // End of include guard
