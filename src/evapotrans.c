/*
 * =====================================================================================
 *
 *       Filename:  evapotrans.c
 *
 *    Description:  Functions to estimate potential evapotranspiration.
 *
 *        Version:  1.0
 *        Created:  14/09/24 07:35:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Luis Morales-Marin, 
 *   Organization:  Universidad Nacional de Colombia
 *
 * =====================================================================================
 */


#include "evapotrans.h"

/*
 * Hamon method to estimate potential evapotranspiration
 */
double hamon_et0(double Tave, double latr, int jday){

    // Solar declination angle (radians)
    double d = 1 + 0.033*cos(2*M_PI*jday/365);

    // Sunset hour angle
    double w = acos(-tan((double)latr)*tan(d)); 

    // Number of daylight hours
    double N = 24*w/M_PI;

    // Saturation vapor pressure es at the mean daily temperature 
    double e_s = 0.6108*exp(17.27*(double)Tave/((double)Tave+237.3));

    // Saturation vapor density
    double Pt = 216.7*e_s/(Tave+273.16);

    return HAMON_C*N*Pt/12;
}

/*
 * Hamon method to estimate potential evapotranspiration
 */
double hamon_et1(const double Tave, const double lat, const int jday){


    double P = asin(0.39795*cos(0.2163108 + 2.0 * atan(0.9671396*tan(0.00860*(double)(jday-186)))));

    /* Daytime lenght */
    double dayLength = 24.0 - (24.0/M_PI)*(acos((sin(0.8333*M_PI/180.0)+sin(lat*M_PI/180.0)*sin(P))/(cos(lat*M_PI/180.0)*cos(P))));

    /* Saturation vapor pressure */
    double eStar = 0.6108*exp((17.27*Tave)/(237.3+Tave));

    /* Potential evapotranspiration */
    return (715.5*dayLength*eStar/24.0)/(Tave + 273.2);
}


/* **********************************
 * FAO PENMAN-MONTEITH METHOD FOR ET
 * **********************************
 */

/*
 * Velocidad de viento a 2 m
 * Input:
 * - z: Altitute [masl]
 * - uz: Velocidad de viento a una altura z (m/s)
 * Output:
 * - u2: Velocidad de viento a 2 m [m/s]
 */
double u2(const double z, const double uz){
    double u2_v = uz * 4.87 /(log(67.8*z - 5.42));
    /* printf("u2 = %lf [m/s] \n", u2_v); */
    return u2_v;
}

/*
 * Presión de saturación de vapor a la temperatura del aire
 * Input:
 * - T: Air temperature [oC]
 * Output:
 * - e0: Presión de saturación de vapor a la temperatura del aire [kPa]
 */
double e0(const double T){
    return 0.6108*exp((17.27*T)/(T+237.3));
}

/*
 * Presión media de vapor de saturación
 * Input:
 * - Tmin: Minimun air temperature [oC]
 * - Tmax: Maximun air temperature [oC]
 * Output:
 * - es: Presión de saturación de vapor a la temperatura del aire [kPa]
 */
double es(const double Tmin, const double Tmax){
    double es_v = (e0(Tmin)+e0(Tmax))*0.5;
    /* printf("es = %lf [kPa] \n", es_v); */
    return es_v;
}

/*
 * Constante psicrométrica
 * Input:
 * - z: Altitute [masl]
 * - Tave_period: Average air temperature for the simulation period [oC] 
 * Output:
 * - gamma: Psychometric constant [kPa°C-1]
 */
double psychrometric_gamma(const double z, const double Tave_period){
    /* Constants */
    /* const double lambda = 2.45;  [> Calor latente de vaporización [MJ kg-1] <] */
    /* const double cp = 0.001013; [> Calor específico a presión constante [MJ kg-1 °C-1] <] */
    /* const double epsilon = 0.622; [> Cociente del peso molecular de vapor de agua/aire seco <] */

    /* Presión atmosférica [kPa] */
    /* double P = 101.3*pow( ((293-0.0065*z)/293), 5.26); */
    double P = 101.3*pow( ((273.16+Tave_period-0.0065*z)/293), 5.26);
    /* printf("- P = %lf [kPa] \n", P); */
    
    double gamma_v = (CP * P)/(EPSILON * LAMBDA);
    /* printf("gamma = %lf [kPa oC-1] \n", gamma_v); */
    return gamma_v; 
}

/*
 * Pendiente de la curva de presión de saturación de vapor
 * Input:
 * - T: Air temperature [oC] 
 * Output:
 * - Delta: Pendiente de la curva de presión de saturación de vapor [kPa °C-1 ]
 */
double Delta(const double T){

    double Delta_v = 4098*(0.6108*exp((17.27*T)/(T+237.3)))/((T+237.3)*(T+237.3));
    /* printf("Delta = %lf [kPa oC-1] \n", Delta_v); */
    return Delta_v;
}

/*
 * Presión real de vapor 
 * Input:
 * - Tdew: Dew air temperature [oC] 
 * Output:
 * - ea: Presión real de vapor [kPa]
 */
double ea(const double Tdew){

    double ea_v = 0.6108*exp((17.27*Tdew)/(Tdew+237.3));
    /* printf("ea = %lf [kPa] \n", ea_v); */
    return ea_v;
}


/*
 * Radiación extraterrestre para periodos diarios
 * Input:
 * - jday: Day of the year (between 1 a 365)
 * - lat: Latitude [deg]
 * Output:
 * - Ra: Radiación extraterrestre para periodos diarios [MJ m-2 día-1]
 */
double Ra(const int jday, const double lat){
    /* Constants */
    /* const double Gsc = 0.082; [> Constante solar [MJ m-2 min-1] <] */

    /* Distancia relativa inversa Tierra-Sol */
    double dr = 1 + 0.033*cos(2*M_PI*jday/365);

    /* Declinación solar [rad] */
    double delta = 0.409*sin((2*M_PI*jday/365)-1.39);

    /* Angulo de radiación a la puesta del sol [rad] */
    double phi = lat*M_PI/180;
    double omega_s = acos(-tan(phi)*tan(delta));

    double Ra_v = (24*60/M_PI)*GSC*dr*((omega_s*sin(phi)*sin(delta)) + (cos(phi)*cos(delta)*sin(omega_s)));
    /* printf("Ra = %lf [MJ m-2 día-1] \n", Ra_v); */
    return Ra_v;

}


/*
 * Radiación solar entrante (derivada de las diferencias térmicas)
 * Input:
 * - Tmin: Minimun air temperature [oC]
 * - Tmax: Maximun air temperature [oC]
 * - Ra: Radiación extraterrestre para periodos diarios [MJ m-2 día-1]
 * Output:
 * - Rs: Radiación solar entrante [MJ m-2d-1]
 */
double Rs(const double Tmin, const double Tmax, const double Ra){
    /* Constants */
    /* const double krs = 0.16; [> coeficiente de ajuste para localizaciones interiores (inland)  [°C-0.5] <] */

    double Rs_v = KRS*sqrt(Tmax - Tmin)*Ra;
    /* printf("Rs = %lf [MJ m-2 día-1] \n", Rs_v); */
    return Rs_v;
}


/*
 * Radiación neta solar o de onda corta
 * Input:
 * - Rs: Radiación solar entrante [MJ m-2d-1]
 * - alpha: Albedo o coeficiente de reflexión del cultivo.
 * Output:
 * - Rns: Radiación neta solar o de onda corta [MJ m-2d-1]
 */
double Rns(const double Rs, const double alpha){

    double Rns_v = (1-alpha)*Rs;
    /* printf("Rns = %lf [MJ m-2 día-1] \n", Rns_v); */
    return Rns_v;
}

/*
 * Radiación solar en un día despejado
 * Input:
 * - Ra: Radiación extraterrestre para periodos diarios [MJ m-2 día-1]
 * - z: Altitute [masl]
 * Output:
 * - Rso: Radiación neta solar o de onda corta [MJ m-2d-1]
 */
double Rso(const double Ra, const double z){
    double Rso_v = (0.75 + 0.00002*z)*Ra;
    /* printf("Rso = %lf [MJ m-2 día-1] \n", Rso_v); */
    return Rso_v;
}

/*
 * Radiación neta de onda larga
 * Input:
 * - Tmin: Minimun air temperature [oC]
 * - Tmax: Maximun air temperature [oC]
 * - Rs: Radiación solar entrante [MJ m-2d-1]
 * - Rso: Radiación neta solar o de onda corta [MJ m-2d-1]
 * - ea: Presión de vapor real [kPa]
 * Output:
 * - Rnl: Radiación neta de onda larga [MJ m-2d-1]
 */
double Rnl(const double Tmin, const double Tmax, const double Rs, const double Rso, const double ea){
    /* Constants */
    /* const double sigma =4.903E-9; [> Constante de Stefan-Boltzmann [MJ K-4 m-2día-1] <] */

    /* Converting from oC to oK */
    double TmaxK = Tmax+273.15;
    double TminK = Tmin+273.15;

    double TmaxK_4 = TmaxK*TmaxK*TmaxK*TmaxK;
    double TminK_4 = TminK*TminK*TminK*TminK;

    double Rnl_v = SIGMA*(TmaxK_4 + TminK_4)*0.5*(0.34-0.14*sqrt(ea))*((1.35*Rs/Rso)-0.35);
    /* printf("Rnl = %lf [MJ m-2 día-1] \n", Rnl_v); */
    return Rnl_v;
}

/*
 * Radiación neta
 * Input:
 * - Rns: Radiación neta solar o de onda corta [MJ m-2d-1]
 * - Rnl: Radiación neta de onda larga [MJ m-2d-1]
 * Output:
 * - Rn: Radiación neta  [MJ m-2d-1]
 */
double Rn(const double Rns, const double Rnl){
    double Rn_v = Rns - Rnl;
    /* printf("Rn = %lf [MJ m-2 día-1] \n", Rn_v); */
    return Rn_v;
}


/*
 * FAO Penman-Monteith method to estimate potential evapotranspiration
 * Input:
 * - z: Altitute [masl]
 * - T: Temperatura media del aire a 2 m de altura [°C] 
 * - Tmin: Minimun air temperature [oC]
 * - Tmax: Maximun air temperature [oC]
 * - lat: Latitude [deg]
 * - jday: Day of the year (between 1 a 365)
 * - alpha: Albedo o coeficiente de reflexión del cultivo.
 * - uz: Velocidad del viento a z m de altura [m s-1]
 * - u_alt: Altura a la cual el viento es medido a partir del suelo [m]
 * - gamma: Psychometric constant [kPa°C-1]
 * Output:
 * - ETo: evapotranspiración de referencia [mm día-1]
 */
double fao_penman_monteith_et(const double z, const double T, const double Tmin, const double Tmax, const double lat, const int jday, const double alpha, const double uz, const double u_alt, const double gamma){

    /* Constants */
    /* const double G = 0.0; [> Flujo de calor del suelo [MJ m-2 día-1]. For daily simulations is equal to 0 because head fluxes is small from the reference surface. <] */

    /* Delta: Pendiente de la curva de presión de saturación de vapor [kPa oC-1 ] */
    double Delta_v = Delta(T);

    /* Ra: Radiación extraterrestre para periodos diarios [MJ m-2 día-1] */
    double Ra_v = Ra(jday, lat);

    /* Rs: Radiación solar entrante [MJ m-2d-1] */
    double Rs_v = Rs(Tmin,  Tmax, Ra_v);

    /* Rns: Radiación neta solar o de onda corta [MJ m-2d-1] */
    double Rns_v = Rns(Rs_v, alpha);

    /* Rso: Radiación neta solar o de onda corta [MJ m-2d-1] */
    double Rso_v = Rso(Ra_v,  z);

    /* ea: Presión real de vapor [kPa] */
    double ea_v = ea(Tmin);

    /* Rnl: Radiación neta de onda larga [MJ m-2d-1] */
    double Rnl_v = Rnl(Tmin, Tmax,  Rs_v, Rso_v,  ea_v);

    /* Rn: Radiación neta  [MJ m-2d-1] */
    double Rn_v = Rn(Rns_v, Rnl_v);

    /* es: Presión de saturación de vapor a la temperatura del aire [kPa] */
    double es_v = es(Tmin, Tmax);

    /* u2: Velocidad de viento a 2 m [m/s] */
    double u2_v = u2(u_alt, uz);

    /* Reference evapotranspiration (mm d^-1)*/
    return (0.408*Delta_v*(Rn_v-G)+gamma*(900/(T+273))*u2_v*(es_v-ea_v))/(Delta_v + gamma*(1+0.34*u2_v));
    
}


/*
 * Hargreaves method to estimate potential evapotranspiration
 */
/* float hargreaves_et(){ */
/* } */


/*
 *  Potential evapotranspiration for multiple input data
 */
/* int ets(evapot *evp, meteoin *metin, float lat, int ntimes){ */
int ets(const char *etmethod, const double *tmin, const double *tave, const double *tmax, const double *uz, struct tm *timestamp, const double lat, const unsigned int ntimes, const double z, const double albedo, const double u_alt, double *et){

    if (strcmp(etmethod, "hamon0") == 0) {
        printf("ET computed using Hamon 0 method\n");
        
        // Latitude to radians
        double latr = lat * M_PI / 180.0;
        
        // Estimation of ET 
        /* float Tave; */
        int yday;
        unsigned int i;
        for(i = 0; i<ntimes; i++){
            /* Tave = metin[i].tave; */
            /* jday = metin[i].timestamp.tm_yday+1;  */
            yday = timestamp[i].tm_yday+1;// +1 because the counter of yday start at 0. */
            et[i]= hamon_et0(tave[i], latr, yday); 
        }

    } else if (strcmp(etmethod, "hamon1") == 0) {
        printf("ET computed using Hamon 1 method\n");

        // Estimation of ET 
        int yday;
        unsigned int i;
        for(i = 0; i<ntimes; i++){
            yday = timestamp[i].tm_yday+1;// +1 because the counter of yday start at 0. */
            et[i]= hamon_et1(tave[i], lat, yday); 
            /* printf("ET = %lf, Tave = %lf, yday = %d, lat = %lf\n", et[i], tave[i], yday, lat); */
        }

    } else if (strcmp(etmethod, "pen-mon") == 0) {
        printf("ET computed using FAO Penman-Monteith method\n");

        /* Estimation of the psychrometric constant */
        double Tave_period = mean_double(ntimes, tave); /* Estimation the averate temperature during the simulations period */
        double gamma_v = psychrometric_gamma(z, Tave_period);

        // Estimation of ET 
        int yday;
        unsigned int i;
        for(i = 0; i<ntimes; i++){
            yday = timestamp[i].tm_yday+1;// +1 because the counter of yday start at 0. */
            et[i] = fao_penman_monteith_et(z, tave[i], tmin[i], tmax[i], lat, yday, albedo, uz[i], u_alt, gamma_v);
            /* printf("ET = %lf, Tave = %lf, yday = %d, lat = %lf\n", et[i], tave[i], yday, lat); */
        }


/*     } else if (strcmp(etmethod, "hargreaves") == 0) { */
        /* printf("ET computed using Hargreaves method\n"); */

    } else {
        printf("ET observations\n");
    }
    return 0;
}
