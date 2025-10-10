/*
 * =====================================================================================
 *
 *       Filename:  evapotrans.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  14/09/24 07:49:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Luis Morales-Marin, 
 *   Organization:  Universidad Nacional de Colombia
 *
 * =====================================================================================
 */

#ifndef EVAPOTRANS_H  // Check if MY_HEADER_H is not defined
#define EVAPOTRANS_H  // Define MY_HEADER_H

#include "readin.h"
 
#include <stdio.h>
#include <string.h>
#include <math.h>

/*
 * Defining constants
 */
#define HAMON_C 0.1651


/*
 * Defining object types
 */
/* char etmethod[10]; */
/* float *et; */

/* typedef struct { */
    /* float et; */
/* } evapotrans; */

/*
 * Function prototypes
 */
// int ets(evapot *evp, meteoin *metin, float lat, int ntimes);
int ets(char *etmethod, double *tave, struct tm *datestamp, double lat, int ntimes, double *et);

#endif // End of include guard
