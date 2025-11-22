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
void save_model_results(char *filename, const int n, struct tm *timestamp, const double *et, const modstvar *mostv, const snowstvar *sstvar){

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    // Write header
    #if SNOWM == 0 // NO SNOW CALCULATIONS
        fprintf(fp, "datetime,et_mm,pn_mm,ps_mm,pr_mm,en_mm,es_mm,s_mm,r_mm,perc_mm,f_mm,qa_mm,qb_mm,qr_mm,qd_mm,q_mm\n");

    #elif SNOWM == 1 // 
        fprintf(fp, "datetime,rainfall_mm,snowfall_mm,sno_mm,tsnow_oC,snomlt_mm,eres_mm,et_mm,pn_mm,ps_mm,pr_mm,en_mm,es_mm,s_mm,r_mm,perc_mm,f_mm,qa_mm,qb_mm,qr_mm,qd_mm,q_mm\n");
    #endif

    unsigned int i;
    char buffer[30];
    for (i = 0; i < n; i++) {
        /* printf("year: %d, month: %d, day: %d, jday: %d\n", timestamp[i].tm_year, timestamp[i].tm_mon,timestamp[i].tm_mday, timestamp[i].tm_yday+1); */
        // Format timestamp as YYYY-MM-DD HH:MM:SS
        timestamp[i].tm_year -= 1900;
        timestamp[i].tm_mon -= 1;
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timestamp[i]);


        #if SNOWM == 0 // NO SNOW CALCULATIONS
            fprintf(fp, "%s,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf\n", buffer, et[i], mostv[i].pn, mostv[i].ps, mostv[i].pr, mostv[i].en, mostv[i].es, mostv[i].s, mostv[i].r, mostv[i].perc, mostv[i].f, mostv[i].qa, mostv[i].qb, mostv[i].qr, mostv[i].qd, mostv[i].q);

        #elif SNOWM == 1 // 
            fprintf(fp, "%s,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf\n", buffer, sstvar->rainfall[i], sstvar->snowfall[i], sstvar->sno[i], sstvar->tsnow[i], sstvar->snomlt[i], sstvar->eres[i], et[i], mostv[i].pn, mostv[i].ps, mostv[i].pr, mostv[i].en, mostv[i].es, mostv[i].s, mostv[i].r, mostv[i].perc, mostv[i].f, mostv[i].qa, mostv[i].qb, mostv[i].qr, mostv[i].qd, mostv[i].q);
    }

        #endif
    fclose(fp);
    /* printf("Time series written to %s\n", filename); */

}
