/*
 * =====================================================================================
 *
 *       Filename:  writeout.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/10/25 21:08:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef WRITEOUT_H  // Check if MY_HEADER_H is not defined
#define WRITEOUT_H  // Define MY_HEADER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
 * Defining object types
 */
typedef struct {

    #if MODEL == 1 // GR4J
    
        double pn;
        double ps;
        double pr;
        double en;
        double es;
        double s;
        double r;
        double perc;
        double f;
        double qa;
        double qb;
        double qr;
        double qd;
        double q;
   
    #elif MODEL == 2 // HBV
    
    
    #elif MODEL == 3 // HYMOD
    
    
    #else // IAHCRES
    
    
    #endif
} modstvar;



/*
 * Function prototypes
 */
// void save_model_results(char *filename, int n, struct tm *timestamp, double *et, double *runoff);
void save_model_results(char *filename, int n, struct tm *timestamp, double *et, modstvar *mostv);

#endif // End of include guard
