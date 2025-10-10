/*
 * =====================================================================================
 *
 *       Filename:  writeout.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/10/25 21:07:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "writeout.h"

/*
 * Print main model results
 */
void save_model_results(char *filename, int n, struct tm *timestamp, double *et, modstvar *mostv){

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    // Write header
    fprintf(fp, "datetime,et_mm,pn_mm,ps_mm,pr_mm,en_mm,es_mm,s_mm,r_mm,perc_mm,f_mm,qa_mm,qb_mm,qr_mm,qd_mm,q_mm\n");

    unsigned int i;
    char buffer[30];
    for (i = 0; i < n; i++) {
        /* printf("year: %d, month: %d, day: %d, jday: %d\n", timestamp[i].tm_year, timestamp[i].tm_mon,timestamp[i].tm_mday, timestamp[i].tm_yday+1); */
        // Format timestamp as YYYY-MM-DD HH:MM:SS
        timestamp[i].tm_year -= 1900;
        timestamp[i].tm_mon -= 1;
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timestamp[i]);

        fprintf(fp, "%s,%.8f,%.8f,%.8f,%.8f,%.8f,%.8f,%.8f,%.8f,%.8f,%.8f,%.8f,%.8f,%.8f,%.8f,%.8f\n", buffer, et[i], mostv[i]->pn, mostv[i]->ps, mostv[i]->pr, mostv[i]->en, mostv[i]->es, mostv[i]->s, mostv[i]->r, mostv[i]->perc, mostv[i]->f, mostv[i]->qa, mostv[i]->qb, mostv[i]->qr, mostv[i]->qd, mostv[i]->q);

    }

    fclose(fp);
    /* printf("Time series written to %s\n", filename); */

}
