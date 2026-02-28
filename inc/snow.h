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

#if MODEL == 1 // GR4J
               
    void snowModel(const unsigned int i, struct tm *timestamp, double *snowfall, const double *tav, const double *tmax, const double *ep, double *tsnow, double *sno, double *snomlt, double *eres, const modparam *modp,  double cov1, double cov2, double *sno0, double *tsnow0);

#elif MODEL == 2 // HBV

    double snowModel(const unsigned int i, const double tav, const double precip, double *sdep, const double ttlim, const double degw, const double degd);

#elif MODEL == 3 // HYMOD

    double snowModel(const unsigned int i, const double tav, const double precip, double *snow, double *snow_store, double *melt, const double tth, const double tb, const double ddf);

#else // IAHCRES

#endif

#endif // End of include guard
