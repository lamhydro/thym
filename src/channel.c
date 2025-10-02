/*
 * =====================================================================================
 *
 *       Filename:  channel.c
 *
 *    Description:  
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
#include "channel.h"
              
#if MODEL == 1 // GR4J

/*
 * UH1
 */
int uh1_f(int x4, float *uh1){
    if (x4 == 0) {
        printf("Error: Division by zero detected!\n");
        return 1; // Return NaN (Not a Number) to indicate an error
    }
    int t;
    float sh1[x4+1];
    for (t = 0; t <= x4; t++){
      sh1[t] = pow(t/x4,(5.0/2.0));
    }

    for (t = 1; t <= x4; t++){
      uh1[t-1] = sh1[t]-sh1[t-1];
    }
    return 0;
}

/*
 * UH2
 */
int uh2_f(int x4, float *uh2){
    if (x4 == 0) {
        printf("Error: Division by zero detected!\n");
        return 1; // Return NaN (Not a Number) to indicate an error
    }
    int t = 0;
    float sh2[2*x4+1];
    while (t <= x4){
        sh2[t] = 0.5*pow(t/x4,(5.0/2.0));
        t++;
    }
    while (t > x4 && t <= 2*x4){
        sh2[t] = 1 - 0.5*pow(2 - (t/x4),(5.0/2.0));
        t++;
    }
    for (t = 1; t <= 2*x4; t++){
      uh2[t-1] = sh2[t]-sh2[t-1];
    }

    return 0;
}

/*
 * Qa
 */
float Qa_f(float Pr, float *uh1, int x4){   
   int t;
   float Qa = 0;
   float dummy = PA * Pr;
   for (t = 0; t < x4; t++){
       Qa += dummy*uh1[t];
   }
   return Qa;
}

/*
 * Qb
 */
float Qb_f(float Pr, float *uh2, int x4){   
   int t;
   float Qb = 0;
   float dummy = PB * Pr;
   for (t = 0; t < 2*x4; t++){
       Qb += dummy*uh2[t];
   }
   return Qb;
}

#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif




