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
 * Defining constants for Hamon
 */
#define HAMON_C 0.1651

/*
 * Defining constants for Penman-Monteith
 */
#define LAMBDA   2.45  /* Calor latente de vaporización [MJ kg-1] */
#define CP       0.001013 /* Calor específico a presión constante [MJ kg-1 °C-1] */
#define EPSILON  0.622 /* Cociente del peso molecular de vapor de agua/aire seco */
#define GSC      0.082 /* Constante solar [MJ m-2 min-1] */
#define KRS      0.16 /* coeficiente de ajuste para localizaciones interiores (inland)  [°C-0.5] */
#define SIGMA    4.903E-9 /* Constante de Stefan-Boltzmann [MJ K-4 m-2día-1] */
#define G        0.0 /* Flujo de calor del suelo [MJ m-2 día-1]. For daily simulations is equal to 0 because head fluxes is small from the reference surface. */


/*
 * Function prototypes
 */
int ets(const char *etmethod, const double *tmin, const double *tave, const double *tmax, const double *uz, struct tm *timestamp, const double lat, const unsigned int ntimes, const double z, const double albedo, const double u_alt, double *et);

#endif // End of include guard
