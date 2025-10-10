/*
 * =====================================================================================
 *
 *       Filename:  utils.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  14/09/24 07:49:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/*
 * Defining constants
 */
#define BUFFER_LENGTH 1024


/*
 * Function prototypes
 */
char *concat_strings(char *str1, char* str2,  char* sep);
int cpstring(char *source, char *destin);
char *allostr(char *str);
ssize_t my_getline(char **lineptr, size_t *n, FILE *stream);
char *my_getline_to_char(char ch, FILE *stream);
char **get_strings_into_array(char *line, int *nwords);
int get_file_nlines(char *filename, int nheaderl);
int get_time_index(int nts, struct tm *dts, struct tm dt);
void print_box_banner(const char *title, const char *author, const char *organiz);
void print_centered(const char *text, int width);
void freememo(struct tm *v1, double *v2, double *v3, double *v4, double *v5, double *v6);
// int floatToNextInt(float x);
