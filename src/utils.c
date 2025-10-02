/*
 * =====================================================================================
 *
 *       Filename:  utils.c
 *
 *    Description:  Utility function library 
 *
 *        Version:  1.0
 *        Created:  14/09/24 07:35:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "utils.h"

/*
 * Allocate memory for a string
 */
char *allostr(char *str){

    //printf("Argument %s\n", testcase);
    size_t len = strlen(str);  // Find the length of the source string
    /* printf("%ld\n",len); */
       
    // Allocate memory for the destination string (including the null terminator)
    char *astr = (char *)malloc((len + 1) * sizeof(char)); 

    if (astr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    return astr;
}

/*
 * Copy one source string into destination string.
 */
int cpstring(char *source, char *destin) {
    // Source string (can be unknown in size)
    //char source[] = "Hello, dynamic memory allocation!";
    
    // Dynamically allocate memory for the destination string
    // We add 1 to account for the null terminator '\0'
    destin = (char *)malloc((strlen(source) + 1) * sizeof(char));

    // Check if memory allocation was successful
    if (destin == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Copy the string from source to destination
    strcpy(destin, source);

    // Print the copied string
    printf("Copied String: %s\n", destin);

    // Free the allocated memory after use
    //free(destination);

    return 0;
}

/*
 * Concatenate two strings separated by a sep (e.g '/')
 */
char *concat_strings(char *str1, char *str2, char *sep) {
    // Two strings to concatenate
    //char str1[] = "home";
    //char str2[] = "user";
    
    // Calculate the total length needed (length of str1 + length of str2 + 1 for '/' + 1 for null terminator)
    int totalLength = strlen(str1) + strlen(str2) + 2;  // 1 for '/' and 1 for '\0'
    
    // Allocate memory for the new concatenated string
    char *strcc= (char *)malloc(totalLength * sizeof(char));

    if (strcc == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // Copy the first string into the result
    strcpy(strcc, str1);

    // Manually insert the '/' separator
    //strcat(strcc, (char*)sep);
    strcat(strcc, sep); 

    // Concatenate the second string to the result
    strcat(strcc, str2);

    // Print the concatenated string
    /* printf("Concatenated String: %s\n", strcc); */

    // Free the allocated memory
    //free(result);

    return strcc;
}

/*
 * Custom getline function using fgets
 */
ssize_t my_getline(char **lineptr, size_t *n, FILE *stream) {
    size_t pos;
    int c;

    if (*lineptr == NULL) {
        *n = 128;  // Default size for the buffer
        *lineptr = malloc(*n);
        if (*lineptr == NULL) {
            return -1;
        }
    }

    pos = 0;
    while ((c = fgetc(stream)) != EOF) {
        if (pos + 1 >= *n) {
            *n *= 2;
            *lineptr = realloc(*lineptr, *n);
            if (*lineptr == NULL) {
                return -1;
            }
        }

        (*lineptr)[pos++] = c;
        if (c == '\n') {
            break;
        }
    }

    if (pos == 0 && c == EOF) {
        return -1;  // No more lines
    }

    (*lineptr)[pos] = '\0';
    return pos;
}

/*
 * Truncate a line read by my_getline upon a character 'ch'
 */
char *my_getline_to_char(char ch, FILE *stream){
    
    char *line = NULL;  // Pointer to hold the line
    size_t len = 0;     // Size of the buffer
    ssize_t read;       // Number of characters read

    // Read the first line of the file
    read = my_getline(&line, &len, stream);

    // Print the results
    if (read != -1) {
        printf("Read %zd characters: %s", read, line);
    } else {
        printf("Failed to read line or end of file.\n");
    }
    // Find the position of the character we want to stop at (e.g., ',')
    //char *pos = strchr(line1, '!');
    char *pos = strchr(line, ch);
    if (pos != NULL) {
        *pos = '\0';  // Truncate the line at the ch  by replacing it with a null terminator
    }
    return line;
}

/*
 * Save strings in 'line' into an array of strings 'strarray'
 */
char **get_strings_into_array(char *line, int *nwords){

    // Count the number of words by tokenizing the line once
    int wordCount = 0;
    char *tempLine = strdup(line);  // Duplicate the line for safe tokenization
    char *token = strtok(tempLine, " \n");

    // Loop through each token to count the words
    while (token != NULL) {
        wordCount++;
        token = strtok(NULL, " \n");
    }

    free(tempLine);  // Free the duplicated line

    // Now allocate memory for the words dynamically
    char **strarray = (char **)malloc(wordCount * sizeof(char *));
    token = strtok(line, " \n");  // Tokenize the original line again

    // Store the words in the dynamically allocated array
    int index = 0;
    while (token != NULL) {
        //words[index] = (char *)malloc(strlen(token) + 1);
        strarray[index] = (char *)malloc(strlen(token) + 1);
        //strcpy(words[index], token);
        strcpy(strarray[index], token);
        token = strtok(NULL, " \n");
        index++;
    }
    *nwords = index;

    // Output the words to verify they were read correctly
/*     printf("Words read from file:\n"); */
    /* for (int i = 0; i < wordCount; i++) { */
        /* printf("%s\n", strarray[i]); */
        /* //free(strarray[i]);  // Free the allocated memory for each word */
    /* } */

    // Free the array of words
    //free(strarray);
    return strarray;
}

/*
 * Get the number of lines in a file
 */
int get_file_nlines(char *filename, int nheaderl){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    }

    // Read header lines
    char buffer[BUFFER_LENGTH];
    for (int i=0; i < nheaderl; i++){
        fgets(buffer, sizeof(buffer), file);
    }
    // Get number of file lines
    int nlines = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        nlines++;
    }

    fclose(file);
    return nlines;
}

/*
 * Get the position within the tm ts array that match dt (datetime)
 */
int get_time_index(int nts, struct tm *dts, struct tm dt){
    
    time_t time1, time2;
    double diff;

    // Convert struct tm to time_t for comparison
    //dt.tm_year -= 1900;
    //dt.tm_mon -= 1;
    time1 = mktime(&dt);
    /* printf("%d, %d\n", nts, time1); */

    // Get the position where both match
    int i;
    struct tm datt = {0};
    for (i = 0; i < nts; i++){
       datt.tm_year = dts[i].tm_year;// - 1900;
       datt.tm_mon = dts[i].tm_mon;// - 1;
       datt.tm_mday = dts[i].tm_mday;
       datt.tm_hour = dts[i].tm_hour;
       datt.tm_min = dts[i].tm_min;
       datt.tm_sec = dts[i].tm_sec;
       
/*        printf(" Date time is: %d / %d / %d  %d : %d : %d\n", dts[i].tm_year, dts[i].tm_mon, dts[i].tm_mday, dts[i].tm_hour, dts[i].tm_min, dts[i].tm_sec); */
       /* printf(" Date time is: %d / %d / %d  %d : %d : %d\n", dt.tm_year, dt.tm_mon, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec); */

        // Convert struct tm to time_t for comparison
        time2 = mktime(&datt);

        // Use difftime() to calculate the difference
        diff = difftime(time2, time1);
        //printf("the diff is: %lf, time1: %ld, time2: %ld \n", diff, time1, time2);
        
        if (diff == 0) {
            return i;
        }

    }
    return -1;
}

/*
 * Print a banner with basic information
 */
void print_box_banner(const char *title, const char *author, const char *organiz) {
    // Find longest line length (for width)
    int len_author = strlen(author);
    int len_organiz   = strlen(organiz);
    int max_len = strlen(title);
    if (len_author > max_len) max_len = len_author;
    if (len_organiz > max_len)   max_len = len_organiz;

    int padding = 2;  
    int width = max_len + padding * 2 + 2; // box width

    // Top border
    for (int i = 0; i < width; i++) printf("*");
    printf("\n");

    // Empty line
    printf("*%*s*\n", width - 2, "");

    // Title (centered)
    printf("*  ");
    print_centered(title, max_len);
    printf("  *\n");

    // Author (left-aligned)
    /* printf("*  %-*s  *\n", max_len, author); */
    printf("*  ");
    print_centered(author, max_len);
    printf("  *\n");

    // Date (left-aligned)
    /* printf("*  %-*s  *\n", max_len, organiz); */
    printf("*  ");
    print_centered(organiz, max_len);
    printf("  *\n");

    // Empty line
    printf("*%*s*\n", width - 2, "");

    // Bottom border
    for (int i = 0; i < width; i++) printf("*");
    printf("\n");
}


/*
 * Function to centered a text in banner
 */
void print_centered(const char *text, int width) {
    int len = strlen(text);
    if (len >= width) {
        printf("%s", text); // if longer, just print as is
        return;
    }
    int padding = width - len;
    int left = padding / 2;
    int right = padding - left;

    for (int i = 0; i < left; i++) printf(" ");
    printf("%s", text);
    for (int i = 0; i < right; i++) printf(" ");
}

/*
 * Free memory from a series of 1d arrays
 */
void freememo(struct tm *v1, float *v2, float *v3, float *v4, float *v5, float *v6){
   free(v1); 
   free(v2); 
   free(v3); 
   free(v4); 
   free(v5); 
   free(v6); 
}

/*
 *  Approximate a float number to next smaller integer e.g. 4.2 to 5
 */
int floatToNextInt(float x){
    int nextInt;
    return nextInt = (int)ceil(x);
}

/*
 * Read a line from a text file
 */
/* char *read_line(FILE *file, int *len){ */

    /* // Use getline() to read the line dynamically */
    /* char *line = NULL;  // Pointer to hold the line */
    /* size_t len = 0;     // Size of the allocated buffer */
    /* ssize_t read;       // Number of characters read */

    /* // Read the line from the file (getline allocates memory automatically) */
    /* read = getline(&line, &len, file); */

    /* // Check if a line was successfully read */
    /* if (read != -1) { */
        /* // Process the line (e.g., split into words) */
        /* printf("Line read: %s\n", line); */
        /* printf("Number of characters in the line: %zd\n", read);  // Print the length of the line */
    /* } else { */
        /* printf("Error: Could not read the line.\n"); */
    /* } */

    /* // Free the dynamically allocated memory */
    /* free(line); */
/* } */

