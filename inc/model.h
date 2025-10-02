/*
 * =====================================================================================
 *
 *       Filename:  model.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  28/03/25 05:34:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef MODEL_H  // Check if MY_HEADER_H is not defined
#define MODEL_H  // Define MY_HEADER_H
                  //
#include <stdlib.h>

#include "readin.h"
#include "intercep.h"
#include "soil_surf.h"
#include "channel.h"
#include "aquifer.h"

#if MODEL == 1 // GR4J

    //int gr4j(float p, float e, float x1, float x2, float x3, int x4);
    // int gr4j(meteoin *metin, evapot *evp, modparam modp, int ntimes);
int gr4j(float *precip, float *et, float x1, float x2, float x3, int x4, int ntimes, float area, int dt, float *disch_s);

#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif

#endif // End of include guard
