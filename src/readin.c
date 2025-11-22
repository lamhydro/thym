/*
 * =====================================================================================
 *
 *       Filename:  readin.c
 *
 *    Description:  Read thym's input files
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

#include "readin.h"
#include "utils.h"

/*
 * Read test-case name and set up meteo.in and ctrl.in files
 */
int read_init(char *testcase, ininfo *info){

    info->tcase = allostr(testcase);
    
    // Copy the source string to the allocated memory
    strcpy(info->tcase,testcase);

    // Print the destination string
    /* printf("Destination String: %s\n", info->tcase); */

    // Set up the meteo.in file
    info->meteoinf = concat_strings(info->tcase, METEOIN_FILE, "/");
    /* printf("%s\n", info->meteoinf); */

    // Set up the ctrl.in file
    info->ctrlinf = concat_strings(info->tcase, CTRLIN_FILE, "/");
    /* printf("%s\n", info->ctrlinf); */
    
    // Set up the results.out file
    info->resultsoutf = concat_strings(info->tcase, RESULTSOUT_FILE, "/");

    // Free the allocated memory
    //free(info.tcase);
    //free(info.meteoinf);

    // Set meteo.in
    
    // Set meteo.in
    return 0;
}

/*
 * Free memory allocated for ininfo struct
 */
void freeininfo(ininfo *info){
    free(info->tcase);
    free(info->meteoinf);
    free(info->ctrlinf);
}

/*
 * Free memory allocated for ctrlout struct
 */
void freectrlout(ctrlout *ctrlo){
    int i;
    for (i = 0; i < ctrlo->nvarout; i++){
        free(ctrlo->varout[i]);
    }
    free(ctrlo->varout);
}

/*
 * Read ctrl.in file exept model name and parameters
 */
//int read_ctrl(char *ctrlf, ctrlbasic *ctrlb, ctrlout *ctrlo, ctrlparam *ctrlp){
int read_ctrl(char *ctrlf, ctrlbasic *ctrlb, ctrlout *ctrlo, modparam *modp, char *etmethod, snowparam *snowp){

    char line[MAX_LINE_LENGTH];

    FILE *file = fopen(ctrlf, "r");
    if (file == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    }else {

        // Read initial block of input information
		printf("\n");
		printf("******* READING BASIC INFORMATION FROM '%s' *******\n", ctrlf);
		printf("\n");

        //ctrlbasic ctrlb;
        // Read watershed name
        fgets(line, sizeof(line), file);
        sscanf(line, "%[^!]", ctrlb->name);
        printf("Catchment name: %s\n", ctrlb->name);

        // Read latitude
        fgets(line, sizeof(line), file);
        sscanf(line, "%lf", &ctrlb->lat);
            printf("Latitude at the catchment outlet (dec): %f\n", ctrlb->lat);

        // Read longitude
        fgets(line, sizeof(line), file);
        sscanf(line, "%lf", &ctrlb->lon);
            printf("Longitude at the catchment outlet (dec): %f\n", ctrlb->lon);

        // Read altitude
        fgets(line, sizeof(line), file);
        sscanf(line, "%lf", &ctrlb->alt);
            printf("Altitude at the catchment outlet (m.a.s.l.): %f\n", ctrlb->alt);

        // Read watershed area 
        fgets(line, sizeof(line), file);
        sscanf(line, "%lf", &ctrlb->area);
        ctrlb->area*=1000000.0; // from km2 to m2
            printf("Catchment area (m^2): %0.2f\n", ctrlb->area);
       
        // Reading start datetime
		fgets(line, sizeof(line), file);
		sscanf(line, "%d %d %d %d %d %d", &ctrlb->startdt.tm_year, &ctrlb->startdt.tm_mon, &ctrlb->startdt.tm_mday, &ctrlb->startdt.tm_hour, &ctrlb->startdt.tm_min, &ctrlb->startdt.tm_sec);
        ctrlb->startdt.tm_isdst = -1;
        /* ctrlb->startdt.tm_year += 1900; */
        /* ctrlb->startdt.tm_mon += 1; */
		if (ctrlb->startdt.tm_hour > 23 || ctrlb->startdt.tm_min > 59 || ctrlb->startdt.tm_sec > 59){
			fprintf(stderr,"Start time out of limits: hou[0-23] min[0-59] sec[0-59], closing ...\n");
			return 1;
		}
		else{
            printf("Start time is: %d / %d / %d  %d : %d : %d\n", ctrlb->startdt.tm_year, ctrlb->startdt.tm_mon, ctrlb->startdt.tm_mday, ctrlb->startdt.tm_hour, ctrlb->startdt.tm_min, ctrlb->startdt.tm_sec);
        }

        // Reading end datetime
		fgets(line, sizeof(line), file);
		sscanf(line, "%d %d %d %d %d %d", &ctrlb->enddt.tm_year, &ctrlb->enddt.tm_mon, &ctrlb->enddt.tm_mday, &ctrlb->enddt.tm_hour, &ctrlb->enddt.tm_min, &ctrlb->enddt.tm_sec);
        ctrlb->enddt.tm_isdst = -1;
        /* ctrlb->enddt.tm_year += 1900; */
        /* ctrlb->enddt.tm_mon += 1; */
		if (ctrlb->enddt.tm_hour > 23 || ctrlb->enddt.tm_min > 59 || ctrlb->enddt.tm_sec > 59){
			fprintf(stderr,"End time out of limits: hou[0-23] min[0-59] sec[0-59], closing ...\n");
			return 1;
		}
		else{
            printf("Start end is: %d / %d / %d  %d : %d : %d\n", ctrlb->enddt.tm_year, ctrlb->enddt.tm_mon, ctrlb->enddt.tm_mday, ctrlb->enddt.tm_hour, ctrlb->enddt.tm_min, ctrlb->enddt.tm_sec);
        }

        // Compare start and end datetime
        
        // Read delta time
        fgets(line, sizeof(line), file);
        sscanf(line, "%d", &ctrlb->dt);
            printf("Timestep is (sec): %d\n", ctrlb->dt);

        fscanf(file,"\n");

        // Read block of information related to output variables
        char *line1 = NULL;  // Pointer to hold the line
        line1 = my_getline_to_char('!',file);

        //ctrlout ctrlo;
        ctrlo->varout = get_strings_into_array(line1, &ctrlo->nvarout);
        free(line1); // Free the memory allocated by getline()
        // Output the words to verify they were read correctly
        printf("Output variables are:\t");
        for (int i = 0; i < 4; i++) {
            printf("%s\t", ctrlo->varout[i]);
            //free(strarray[i]);  // Free the allocated memory for each word
        }
        printf("\n");

        fgets(line, sizeof(line), file);
        sscanf(line, "%d", &ctrlo->dtout);
            printf("Timestep to write output variables: %d\n", ctrlo->dtout);

        fscanf(file,"\n");

        // Read block of information related to parameters
/*         fgets(line, sizeof(line), file); */
        /* sscanf(line, "%f", &ctrlp->param1); */
            /* printf("Parameter 1: %f\n", ctrlp->param1); */

        /* fgets(line, sizeof(line), file); */
        /* sscanf(line, "%f", &ctrlp->param2); */
            /* printf("Parameter 2: %f\n", ctrlp->param2); */

        /* fgets(line, sizeof(line), file); */
        /* sscanf(line, "%f", &ctrlp->param3); */
            /* printf("Parameter 3: %f\n", ctrlp->param3); */

        // Read  evapotranspiration method
        fgets(line, sizeof(line), file);
        sscanf(line, "%9s", etmethod);
        printf("Evapotranspiration method: %s\n", etmethod);

        fscanf(file,"\n");

        // Read hydrological model name
        fgets(line, sizeof(line), file);
        sscanf(line, "%9s", modp->model);
        printf("Hydrological model: %s\n", modp->model);


        fgets(line, sizeof(line), file);
        #if MODEL == 1 // GR4J
            /* float dummy; */
		    sscanf(line, "%lf %lf %lf %lf", &modp->x1, &modp->x2, &modp->x3, &modp->x4);
            /* modp->x4 = floatToNextInt(dummy); // Transform a float to next smallest int */
            printf("x1 = %lf, x2 = %lf, x3 = %lf, x4 = %lf\n", modp->x1, modp->x2, modp->x3, modp->x4);
        #elif MODEL == 2 // HBV
		    sscanf(line, "%lf %lf %lf %lf", &modp->w1, &modp->w2, &modp->w3, &modp->w4);
        #elif MODEL == 3 // HYMOD
		    sscanf(line, "%lf %lf %lf %lf", &modp->y1, &modp->y2, &modp->y3, &modp->y4);
        #else // IAHCRES
		    sscanf(line, "%lf %lf %lf %lf", &modp->z1, &modp->z2, &modp->z3, &modp->z4);
        #endif

        fscanf(file,"\n");

        // Read snow model parameters
        fgets(line, sizeof(line), file);
        #if SNOWM == 0 // NO SNOW CALCULATIONS
        #elif SNOWM == 1 // 
		    sscanf(line, "%lf %lf %lf %lf %lf %lf %lf", &snowp->trs, &snowp->tmlt, &snowp->sno50, &snowp->sno100, &snowp->ls, &snowp->bmlt6, &snowp->bmlt12);
            printf("trs = %lf, tmlt = %lf, sno50 = %lf, sno100 = %lf, ls = %lf, bmlt6 = %lf, bmlt12 = %lf\n", snowp->trs, snowp->tmlt, snowp->sno50, snowp->sno100, snowp->ls, snowp->bmlt6, snowp->bmlt12);
        #else // 
        #endif

		fclose(file);

	}
	return 0;
}

/*
* Get the time interval to read in meteo.in input file
*/
int get_time_interval_meteo_in(char *meteof, struct tm sdt, struct tm edt, meteoini *metinii){
    FILE *file = fopen(meteof, "r");
    if (file == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    }
    
    // Read header lines
    char line[MAX_LINE_LENGTH];
    for (int i=0; i < NHEADER_LINES_METEOIN; i++){
        fgets(line, sizeof(line), file);
    }

    // Coverting start-dt and end-dt to an elapsed time
    sdt.tm_year -= 1900;
    sdt.tm_mon -= 1;
    edt.tm_year -= 1900;
    edt.tm_mon -= 1;
    time_t stime, etime;
    stime = mktime(&sdt);
    etime = mktime(&edt);
    printf("stime: %ld, etime: %ld\n", stime, etime);

    // Read the meteo data    
    int i = NHEADER_LINES_METEOIN;
    char *timestamp_str;
    struct tm dummyt = {0};
    int ntimes = 0; 
    time_t dtime;
    double sdiff, ediff;
    int on = 0;
    ediff = -1.0;
    while (fgets(line, sizeof(line), file)) {
        // Split the line into timestamp and value
        timestamp_str = strtok(line, ",");

        // Parse the timestamp using strptime
        strptime(timestamp_str, "%Y-%m-%d %H:%M:%S", &dummyt);

        // Adjust year and month
        /* dummyt.tm_year += 1900; */
        /* dummyt.tm_mon += 1; */
        dummyt.tm_isdst = -1;
        
/*         printf("Timestamp: %04d-%02d-%02d %02d:%02d:%02d\n", dummyt.tm_year, dummyt.tm_mon, dummyt.tm_mday,dummyt.tm_hour, dummyt.tm_min, dummyt.tm_sec); */
        /* printf("Timestamp_start: %04d-%02d-%02d %02d:%02d:%02d\n", sdt.tm_year, sdt.tm_mon, sdt.tm_mday,sdt.tm_hour, sdt.tm_min, sdt.tm_sec); */
        /* printf("Timestamp_end: %04d-%02d-%02d %02d:%02d:%02d\n", edt.tm_year, edt.tm_mon, edt.tm_mday,edt.tm_hour, edt.tm_min, edt.tm_sec); */

        // Coverting to an elapsed time
        dtime = mktime(&dummyt);
        //printf("dtime: %ld\n", dtime);

        // Use difftime() to calculate the difference
        sdiff = difftime(dtime, stime);
        /* printf("sdiff: %lf\n", sdiff); */

        if (sdiff == 0) {
            /* printf("aaaaaaaaaaaaaaaaaaaaaaa%d\n",i); */
            on = 1;
            metinii->sdt = i;
        } 
        if (on == 1){
            ntimes++;
            ediff = difftime(dtime, etime);
        }
        if (ediff == 0){
            metinii->edt = i;
            metinii->ntimes = ntimes;
            break;
        }
        i++;
    }

    fclose(file);

    return 0;
}

/*
 * Read meteo.in file
 */
int read_meteo(char *meteof, meteoin *metin, meteoini *metini, struct tm *dts, double *et) {

    FILE *file = fopen(meteof, "r");
    if (file == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    }
	printf("\n");
	printf("******* READING HYDROMETEO INFORMACION IN '%s' *******\n", meteof);
	printf("\n");

    // Read header lines
    char line[MAX_LINE_LENGTH];
    for (int i=0; i < NHEADER_LINES_METEOIN; i++){
        fgets(line, sizeof(line), file);
    }

    // Read the meteo data    
    unsigned int i = NHEADER_LINES_METEOIN;
    unsigned int j = 0;
    char *timestamp_str, *value1_str, *value2_str, *value3_str, *value4_str, *value5_str, *value6_str, *value7_str;
    /* printf("sdt: %d, edt: %d, ntimes: %d \n", metini->sdt, metini->edt, metini->ntimes);  */
    while (fgets(line, sizeof(line), file)) {
        if (i >= metini->sdt && i <= metini->edt){

            /* printf("i = %d\n",i); */
            // Split the line into timestamp and value
            timestamp_str = strtok(line, ",");
            value1_str = strtok(NULL, ",");
            value2_str = strtok(NULL, ",");
            value3_str = strtok(NULL, ",");
            value4_str = strtok(NULL, ",");
            value5_str = strtok(NULL, ",");
            value6_str = strtok(NULL, ",");
            /* value7_str = strtok(NULL, ","); */
    
            // Parse the timestamp using strptime
            strptime(timestamp_str, "%Y-%m-%d %H:%M:%S", &metin->timestamp[j]);
            //strptime(timestamp_str, "%Y-%m-%d %H:%M:%S", &dts[i]);

            dts[j].tm_year = metin->timestamp[j].tm_year;
            dts[j].tm_mon = metin->timestamp[j].tm_mon;
            dts[j].tm_mday = metin->timestamp[j].tm_mday;
            dts[j].tm_hour = metin->timestamp[j].tm_hour;
            dts[j].tm_min = metin->timestamp[j].tm_min;
            dts[j].tm_sec = metin->timestamp[j].tm_sec;

            // Normalize & compute yday, wday, etc.
            /* mktime(&dts[j]); */
    
            metin->timestamp[j].tm_year += 1900;
            metin->timestamp[j].tm_mon += 1;
            metin->timestamp[j].tm_isdst = -1;
            
            // Convert value to a double
            metin->tave[j] = atof(value1_str);
            metin->tmin[j] = atof(value2_str);
            metin->tmax[j] = atof(value3_str);
            metin->precip[j] = atof(value4_str);
            et[j] = atof(value5_str);
            metin->runoff[j] = atof(value6_str);
            /* metin->srad[j] = atof(value6_str); */
    
            // Print the parsed data
            /* printf("Timestamp: %04d-%02d-%02d %02d:%02d:%02d,  Temp(oC): %.2f, Precip(mm): %.2f\n", */
                   /* metin[i].timestamp.tm_year, metin[i].timestamp.tm_mon, metin[i].timestamp.tm_mday, */
                   /* metin[i].timestamp.tm_hour, metin[i].timestamp.tm_min, metin[i].timestamp.tm_sec, metin[i].temp, metin[i].precip); */
            j++;
        }
        i++;
    }

    fclose(file);
    return 0;
}

/*
 * Allocate memory fo the meteoin struct
 */
int allocateMemo(int size, meteoin *metin){
    metin->timestamp = (struct tm *)malloc(size * sizeof(struct tm));
    metin->tave = (double *)malloc(size * sizeof(double));
    metin->tmin = (double *)malloc(size * sizeof(double));
    metin->tmax = (double *)malloc(size * sizeof(double));
    metin->precip = (double *)malloc(size * sizeof(double));
    metin->runoff = (double *)malloc(size * sizeof(double));
    // Check if malloc succeeded
    if (!metin->timestamp || !metin->tave || !metin->tmin || !metin->tmax || !metin->precip || !metin->runoff) {
        perror("malloc failed");
        free(metin->timestamp);
        free(metin->tave);
        free(metin->tmin);
        free(metin->tmax);
        free(metin->precip);
        free(metin->runoff);
        /* free(metin->srad); */
        return 1;
    }

    return 0;
}



