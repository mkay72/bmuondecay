#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "fqgrid.h"
#include "constants.h"
#include "error_handler.h"
#include "fqgrid_stack.h"

#include "fqg_name.h"


fqgrid_t* new_fqgrid(char* name)
{
    fqgrid_t* fqgrid = (fqgrid_t*) malloc(1 * sizeof(fqgrid_t));

    // printf("%s\n", name);
    // fqg_name_t* fqg_name = new_fqg_name(name);

    strncpy(fqgrid->name, name, FQGRID_MAX_NAME-1);
    fqgrid->name[FQGRID_MAX_NAME - 1] = '\0';

    determine_qgrid(fqgrid);


    fqgrid->data = (double complex*) calloc(fqgrid->size_tot, sizeof(double complex));
    fqgrid->id = get_unique_id_fqgird();

    
    return fqgrid;

}


fqgrid_t* new_fqgrid_from_fqg_name(fqg_name_t* fqg_name)
{
    fqgrid_t* fqgrid = (fqgrid_t*) malloc(1 * sizeof(fqgrid_t));

    // printf("%s\n", name);
    // fqg_name_t* fqg_name = new_fqg_name(name);

    strncpy(fqgrid->name, fqg_name->name, FQGRID_MAX_NAME-2);
    fqgrid->name[FQGRID_MAX_NAME - 1] = '\0';

    // determine_qgrid(fqgrid);

    determine_qgrid_from_fqg_name(fqgrid, fqg_name);


    fqgrid->data = (double complex*) calloc(fqgrid->size_tot, sizeof(double complex));
    fqgrid->id = get_unique_id_fqgird();

    
    return fqgrid;

}


void delete_fqgrid(fqgrid_t* fqgrid)
{
    free(fqgrid->data);
    free(fqgrid->size);
    free(fqgrid->qgrid);
    free(fqgrid);
}


size_t get_unique_id_fqgird()
{
    static size_t fqgrid_count = 0;

    size_t id = fqgrid_count;
    fqgrid_count++;
    return id;
}


void determine_qgrid(fqgrid_t* fqgrid)
{

    bool is_name_valid = check_name(fqgrid->name);

    int nargs = count_nargs(fqgrid->name);
    char *args[nargs];

    parse_args(fqgrid->name, nargs, args);

    fqgrid->ndim = nargs;
    fqgrid->size = (int*) malloc(nargs * sizeof(int));
    fqgrid->qgrid = (qgrid_t**) malloc(nargs * sizeof(qgrid_t*));

    int size_tot = 1;
    for (int iarg=0; iarg < nargs; iarg++)
    {
        fqgrid->qgrid[iarg] = find_qgrid_stack(args[iarg]);
        if (fqgrid->qgrid[iarg] == NULL)
        {
            error_handler("in determine_qgrid();\nCould not associate fqgrid `%s' with qgrid `%s';\nqgrid `%s' is abscent in the qgrid stack", fqgrid->name, args[iarg], args[iarg]);
        }
        fqgrid->size[iarg] = fqgrid->qgrid[iarg]->n;
        size_tot *= fqgrid->size[iarg];

        free(args[iarg]);
    }
    fqgrid->size_tot = size_tot;

}


void determine_qgrid_from_fqg_name(fqgrid_t* fqgrid, fqg_name_t* fqg_name)
{

    // bool is_name_valid = check_name(fqgrid->name);

    // int nargs = count_nargs(fqgrid->name);
    // char *args[nargs];

    // parse_args(fqgrid->name, nargs, args);


    int nargs = 0;
    for (int i = 0; i < 2; i++)
    {
        if (strlen(fqg_name->inflexion[i]) > 0)
        {
            nargs++;
        }
    }

    fqgrid->ndim = nargs;
    fqgrid->size = (int*) malloc(nargs * sizeof(int));
    fqgrid->qgrid = (qgrid_t**) malloc(nargs * sizeof(qgrid_t*));

    int size_tot = 1;
    for (int iarg=0; iarg < nargs; iarg++)
    {
        fqgrid->qgrid[iarg] = find_qgrid_stack(fqg_name->inflexion[iarg]);
        if (fqgrid->qgrid[iarg] == NULL)
        {
            error_handler("in determine_qgrid();\nCould not associate fqgrid `%s' with qgrid `%s';\n \
                           qgrid `%s' is abscent in the qgrid stack", fqgrid->name, fqg_name->inflexion[iarg], fqg_name->inflexion[iarg]);
        }
        fqgrid->size[iarg] = fqgrid->qgrid[iarg]->n;
        size_tot *= fqgrid->size[iarg];

        // free(args[iarg]);
    }
    fqgrid->size_tot = size_tot;

}



void print_fqgrid(char* name)
{
    fqgrid_t* fqgrid = find_fqgrid_stack(name);

    int size_1 = fqgrid->size[0];


    printf("fqgrid_name: %s\n", fqgrid->name);
    if (fqgrid->ndim == 1)
    {
        for (int j=0; j < size_1; j++)
        {
            printf("(%+8.16e %+8.16e)\n", *(fqgrid->data + j));
        }
    
    }
    else if (fqgrid->ndim == 2)
    {
        int size_2 = fqgrid->size[1];
        for (int i=0; i < size_1; i++)
        {
            for (int j=0; j < size_2; j++)
            {
                // printf("(%+5.4e %+5.4e) ", *(fqgrid->data + i*size_2 + j));
                printf("%+5.4e ", creal(*(fqgrid->data + i*size_2 + j)));
            }
            printf("\n");
        }
    }
    printf("\n");

}



fqgrid_t* readfromfile_fqgrid(char* name, char* fname)
{
    fqgrid_t* fqgrid = (fqgrid_t*) malloc(1 * sizeof(fqgrid_t));
    strncpy(fqgrid->name, name, FQGRID_MAX_NAME);
    fqgrid->name[FQGRID_MAX_NAME - 1] = '\0';

    determine_qgrid(fqgrid);

    FILE *fptr;
    fptr = fopen(fname, "r");

    if(fptr == NULL) 
    {
        printf("Not able to open the file %s", fname);
        fclose(fptr);
    }
    

    fqgrid->data = (double complex*) calloc(fqgrid->size_tot, sizeof(double complex));
    fqgrid->id = get_unique_id_fqgird();

    int iline = 0;
    while (!feof(fptr) && !ferror(fptr))
    {
        fscanf(fptr, "%lf", fqgrid->data + iline);
        iline += 1;
    }

    fclose(fptr);

    
    return fqgrid;

}


// utility functions used locally

int tokenize_str(const char* input, char* tokens[]) 
{
    char* inputCopy = strdup(input);  // Copy of the original string
    int ntok = 0;  // Counter for the number of tokens

    // printf("tokenize_str %s\n", inputCopy);

    char COMM = ',';
    // Tokenize the string using strtok
    char* token = strtok(inputCopy, &COMM);
    // printf("token = %s\n", token);
    while (token != NULL) 
    {
        tokens[ntok] = strdup(token);  // Save a copy of the token in the array
        ntok++;  // Increment the token count

        token = strtok(NULL, &COMM);  // Get the next token
        // printf("token = %s\n", token);
    }

    free(inputCopy);  // Free the memory allocated for the copied string

    return ntok;  // Return the number of tokens
}


int count_nargs(char* str)
{
    int ncomm = 0;
    char str_trimmed[strlen(str)];
    trim_parentheses(str, str_trimmed);

    ncomm = count_commas(str_trimmed);
    return ncomm + 1;
}


void trim_leading_asterisk(char* str) 
{
    static char ASTR = '*';
    if (str != NULL && *str == ASTR) 
    {
        memmove(str, str + 1, strlen(str));
    }
}


void parse_args(char* str, int nargs, char* args[])
{
    char str_trimmed[strlen(str)];
    trim_parentheses(str, str_trimmed);
    int ntok = 0;
    char *token_array[nargs];  

    // printf("%s\n", str);
    char ASTR = '*';

    ntok = tokenize_str(str_trimmed, token_array);
    // printf("str = %s\tnargs = %d\tntok = %d\n", str, nargs, ntok);

    if (nargs != ntok)
    {
        error_handler("parse_args: nargs != ntok");
    }
    if (ntok > 0)
    {
        for (int itok = 0; itok < ntok; itok++)
        {
            char *token_copy = strdup(token_array[itok]);

            char *token_trimmed;
            token_trimmed = strchr(token_copy, ASTR);
            if (token_trimmed == NULL)
            {
                token_trimmed = token_copy;
            }            

            size_t len = strlen(token_trimmed);
            trim_leading_asterisk(token_trimmed);

            args[itok] = strdup(token_trimmed);
            free(token_copy);
            free(token_array[itok]);
        }
    }
}


bool check_parentheses(const char *str) 
{
    int openingCount = 0;
    int closingCount = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '(') {
            openingCount++;
        } else if (str[i] == ')') {
            closingCount++;
        }

        // Check for invalid parentheses
        if (closingCount > openingCount) {
            return false;
        }
    }

    // Check if there is exactly one pair of parentheses
    return openingCount == 1 && closingCount == 1;
}


bool check_name(char* name)
{
    bool has_correct_parentheses = 0;

    // printf("%s \n", name);

    has_correct_parentheses = check_parentheses(name);
    if (has_correct_parentheses != 1)
    {
        error_handler("check_name: invalid name %s", name);
    }
    
    return has_correct_parentheses;
}




void trim_parentheses(const char* str, char* result) 
{
    int len = strlen(str);
    int insideParentheses = 0; // Flag to track whether currently inside parentheses
    int writeIndex = 0; // Index to write trimmed characters

    for (int i = 0; i < len; i++) {
        if (str[i] == '(') {
            insideParentheses = 1; // Set flag to indicate inside parentheses
        } else if (str[i] == ')') {
            insideParentheses = 0; // Reset flag to indicate outside parentheses
        } else if (insideParentheses) {
            result[writeIndex++] = str[i]; // Write characters inside parentheses
        }
    }

    result[writeIndex] = '\0'; // Null-terminate the trimmed string
}

int count_commas(const char* str) 
{
    int len = strlen(str);
    int count = 0;

    for (int i = 0; i < len; i++) {
        if (str[i] == ',') {
            count++;
        }
    }

    return count;
}