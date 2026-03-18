/*
 * =====================================================================================
 *
 *       Filename:  channel.h
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

#ifndef CHANNEL_H  // Check if MY_HEADER_H is not defined
#define CHANNEL_H  // Define MY_HEADER_H
                   
#include <stdio.h>
#include <stdlib.h>
#include <math.h>  

// #define PA 0.9 // Proportion of flow toward the routing store
// #define PB 0.1 // Proportion of flow toward ...
               
#if MODEL == 1 // GR4J
               
    int uh1_f(double x4, double **uh, unsigned int *uh_l);
    int uh2_f(double x4, double **uh, unsigned int *uh_l);
    void print_uh(const double *uh, unsigned int uh_l);

#elif MODEL == 2 // HBV
                 
    void routingModel(const double Qall, const unsigned int j, const int maxbas, double *qrou, double *qsim);
    void backflowModel(const int maxbas, double *qrou);

#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif

#endif // End of include guard


