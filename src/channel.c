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
int uh1_f(double x4, double **uh, int *uh_l){
    if (x4 == 0.0) {
        printf("Error: Division by zero detected!\n");
        return 1; // Return NaN (Not a Number) to indicate an error
    }

    if(x4<= 1.0){
        *uh_l = 1;
        *uh = (double *)malloc(sizeof(double) * *uh_l);
        *(*uh + 0) = 1.0;
    }else{
        *uh_l = (int)ceil(x4);
        double *sh;
        sh = (double *)malloc(sizeof(double) * *uh_l);
        *uh = (double *)malloc(sizeof(double)* *uh_l);
        int t;
        for (t = 0; t < *uh_l; t++){
            if (t + 1 < x4)
            {
                *(sh + t) = pow( (t + 1) / x4, 2.5);
            } else {
                *(sh + t) = 1;
            }
        }
        *(*uh + 0) = *(sh + 0);
        for (t = 1; t < *uh_l; t++)
        {
            *(*uh + t) = *(sh + t) - *(sh + t -1);
        }     
        free(sh);   
    }
/*     for (t = 0; t <= x4i; t++){ */
        /* if(t==0){ */
            /* sh[t] = 0.0; */
        /* }else if(t>0 && t<x4i){ */
            /* sh[t] = pow(t/x4,(5.0/2.0)); */
        /* }else{ */
            /* sh[t] = 1.0; */
        /* } */
    /* } */

    /* printf("x4 = %f, x4i =%d\n", x4, x4i); */
   /*  for (t = 1; t <= x4i; t++){ */
      /* uh[t-1] = sh[t]-sh[t-1]; */
      /* printf("uh1[%d]=%f\n", t-1, uh[t-1]); */
    /* } */
    return 0;
}

/*
 * UH2
 */
int uh2_f(double x4, double **uh, int *uh_l){
    if (x4 == 0.0) {
        printf("Error: Division by zero detected!\n");
        return 1; // Return NaN (Not a Number) to indicate an error
    }

    if (x4 <= 0.5){
        *uh_l = 1;
        *uh = (double *)malloc(sizeof(double) * *uh_l);
        *(*uh + 0) = 1;
    } else if (x4 > 0.5 && x4 < 1)
    {
        *uh_l = 2;
        *uh = (double *)malloc(sizeof(double) * *uh_l);
        *(*uh + 0) = 1 - 0.5 * pow(2 - 1 / x4, 2.5);
        *(*uh + 1) = 1 - *(*uh + 0);
    } else if (x4 == 1)
    {
        *uh_l = 2;
        *uh = (double *)malloc(sizeof(double) * *uh_l);
        *(*uh + 0) = 0.5 * pow(1 / x4, 2.5);
        *(*uh + 1) = 1 - *(*uh + 0);
    } else if (x4 > 1)
    {
        int t;
        double *sh;
        *uh_l = ((int)ceil(x4)) * 2;
        *uh = (double *)malloc(sizeof(double) * *uh_l);
        sh = (double *)malloc(sizeof(double) * *uh_l);
        for (t = 0; t < *uh_l; t++)
        {
            if (t + 1 <= x4)
            {
                *(sh + t) = 0.5 * pow((t + 1) / x4, 2.5); 
            } else if (t + 1 > x4 && t + 1 < 2 * x4)
            {
                *(sh + t) = 1 - 0.5 * pow(2 - (t + 1) / x4, 2.5);
            } else {
                *(sh + t) = 1;
            }
        }
        *(*uh + 0) = *(sh + 0);
        for (t = 1; t < *uh_l; t++)
        {
            *(*uh + t) = *(sh + t) - *(sh + t - 1);
        }
        free(sh);
    }

    /* int t; */
    /* int x4i = (int)ceil(x4); */
    /* int dummy = 2*x4i; */

    /* if (x4 <= 0.5){ */

        /* float sh2[1]; */
    /* } */

    /* for (t = 0; t <= dummy; t++){ */
        /* if(t==0){ */
            /* sh2[t] = 0.0; */
        /* }else if(t>0 && t<=x4i){ */
            /* sh2[t] = 0.5*pow(t/x4,(5.0/2.0)); */
        /* }else if(t>x4i && t<dummy){ */
            /* sh2[t] = 1.0 - 0.5*pow(2.0 - (t/x4),(5.0/2.0)); */
        /* }else{ */
            /* sh2[t] = 1.0; */
        /* } */
        /* printf("sh2[%d] = %f\n", t, sh2[t]); */
    /* } */

    /* printf("2*x4 = %f\n", 2*x4); */
    /* for (t = 1; t <= dummy; t++){ */
      /* uh2[t-1] = sh2[t]-sh2[t-1]; */
      /* printf("uh2[%d]=%f\n", t-1, uh2[t-1]); */
    /* } */

    return 0;
}

/*
 * Printing UH
 */
void print_uh(const double *uh, const int uh_l){
    unsigned int i;
    for (i = 0; i < uh_l; i++)
    {
        printf("%d\t%f\n", i + 1, *(uh + i));
    }
}

/*
 * Q
 */
/* double Q_f(double Pr, double *uh, int uh_l, float per){    */
   /* int t; */
   /* double Q = 0; */
   /* double dummy = per * Pr; */
   /* for (t = 0; t < uh_l; t++){ */
       /* Q += dummy*uh[t]; */
   /* } */
   /* return Q; */
/* } */



#elif MODEL == 2 // HBV


#elif MODEL == 3 // HYMOD


#else // IAHCRES


#endif




