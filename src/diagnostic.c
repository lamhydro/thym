/*
 * =====================================================================================
 *
 *       Filename:  diagnostic.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  29/01/26 07:06:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "diagnostic.h"


/*
 * Nash-Sutcliffe efficiency
 */
double nash_sutcliffe(const unsigned int n, const double *obs, const double *sim){

    double obs_mean = mean_double(n, obs);

    unsigned int i;
    double num = 0;
    double den = 0;
    for(i = 0; i < n; i++){
       num += (sim[i]-obs[i]) * (sim[i]-obs[i]);
       den += (obs_mean-obs[i]) * (obs_mean-obs[i]);
    }

    return 1.0-(num/den);
}


/*
 * Log Nash-Sutcliffe efficiency
 */
double log_nash_sutcliffe(const unsigned int n, const double *obs, const double *sim){

    double log_obs_mean = log(mean_double(n, obs));

    unsigned int i;
    double num = 0;
    double den = 0;
    for(i = 0; i < n; i++){
       num += (log(sim[i])-log(obs[i])) * (log(sim[i])-log(obs[i]));
       den += (log_obs_mean-log(obs[i])) * (log_obs_mean-log(obs[i]));
    }

    return 1.0-(num/den);
}


/*
 * Root-mean-squared error
 */
double root_mean_squared_error(const unsigned int n, const double *obs, const double *sim){

    unsigned int i;
    double sum = 0;
    for(i = 0; i < n; i++){
       sum += (sim[i]-obs[i]) * (sim[i]-obs[i]);
    }

    if (sum >= 0) {
        return sqrt(sum);
    } else {
        printf("Cannot take sqrt of negative number\n");
        return 0;
    }

}


/*
 * Percentage bias
 */
double perc_bias(const unsigned int n, const double *obs, const double *sim){

    unsigned int i;
    double num = 0;
    double den = 0;
    for(i = 0; i < n; i++){
       num += sim[i]-obs[i]; 
       den += obs[i];
    }

    return num/den;
}


/*
 * Average absolute error
 */
double ave_abs_error(const unsigned int n, const double *obs, const double *sim){

    unsigned int i;
    double sum = 0;
    for(i = 0; i < n; i++){
       sum += fabs(sim[i]-obs[i]); 
    }

    return sum/n;
}


/*
 * Maximum absolute error
 */
double max_abs_error(const unsigned int n, const double *obs, const double *sim){

    unsigned int i = 0;
    double dummy;
    double maxv = fabs(sim[i]-obs[i]);
    for(i = 1; i < n; i++){
        dummy = fabs(sim[i]-obs[i]);
        if (maxv < dummy) {
            maxv = dummy;
        }
    }
    return maxv;
}


/*
 * Peak difference
 */
double peak_diff(const unsigned int n, const double *obs, const double *sim){

    unsigned int i = 0;
    double max_sim = sim[i];
    for(i = 1; i < n; i++){
        if (max_sim < sim[i]) {
            max_sim = sim[i];
        }
    }

    double max_obs = obs[0];
    for(i = 1; i < n; i++){
        if (max_obs < obs[i]) {
            max_obs = obs[i];
        }
    }
    
    return max_sim - max_obs;
}


/*
 * Correlation of error
 */
double corr_of_error(const unsigned int n, const double *obs, const double *sim){

    double obs_stddev =  stddev_double(n,obs);
    double sim_stddev =  stddev_double(n,sim);

    unsigned int i;
    double sum = 0;
    for(i = 0; i < n-1; i++){
       sum += (sim[i]-obs[i]) * (sim[i+1]-obs[i+1]);
    }

    return (1.0/(obs_stddev*sim_stddev)) * (1.0/(n-1)) * sum;
}

/*
 * Number of sign changes
 */
int numb_of_sign_ch(const unsigned int n, const double *obs, const double *sim){

    unsigned int i = 0;
    double diff1 = sim[i]-obs[i];
    double diff2;
    int nchanges = 0;
    for(i = 1; i < n; i++){
        diff2 = sim[i]-obs[i];
        if ( (diff1 > 0 && diff2 < 0) || (diff1 < 0 && diff2 > 0) ){
            nchanges += 1;
        } else if ( (diff1 == 0 && diff2 < 0) || (diff1 == 0 && diff2 > 0) ){
            nchanges += 1;
        } else if ( (diff1 > 0 && diff2 == 0) || (diff1 < 0 && diff2 == 0) ){
            nchanges += 1;
        }
        diff1 = diff2;
    }

    return nchanges;
}

