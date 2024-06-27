#ifndef FQGRID_H_INCLUDED
#define FQGRID_H_INCLUDED

#include "qgrid.h"
#include <stddef.h>
#include <stdbool.h>
#include <complex.h>

#define FQGRID_NAME_RADICAL_MAX 64
#define FQGRID_NAME_SUFFIX_MAX 32
#define FQGRID_NAME_INFLEXION_MAX 32
#define FQGRID_MAX_NAME 128

typedef struct
{
    size_t id;
    // ndim=1 -- vector, ndim=2 -- matrix
    int ndim;
    // number of elements in each dimension
    int *size;
    int size_tot;
    // buffer array with data
    double complex* data;
    // total number of elements

    // pointers to the quadrature grids on which the function values are calculated
    qgrid_t** qgrid;

    // symbolic name of fqgrid to search it in stack
    char name[FQGRID_MAX_NAME];

} fqgrid_t;


// fqgrid_t* new_fqgrid(qgrid_t* qgrid_1, qgrid_t* qgrid_2, char* name);
fqgrid_t* new_fqgrid(char* name);

void delete_fqgrid(fqgrid_t* fqgrid);

size_t get_unique_id_fqgird();

void print_fqgrid(char* name);

fqgrid_t* readfromfile_fqgrid(char* name, char* fname);

// int count_commas(const char* str);
// void trim_parentheses(const char* str, char* result);
// // void extract_subsring(const char* str, int index, char* result);
// void trim_left_astr(char* str);

// char* get_token(const char* str, int index, char* buffer, size_t bufferSize, const char* delimiter);


void trim_parentheses(const char* str, char* result);
bool check_parentheses(const char *str);
bool check_name(char* name);
void determine_qgrid(fqgrid_t* fqgrid);

int count_commas(const char* str);
int tokenize_str(const char* input, char* tokens[]);
int count_nargs(char* str);
void trim_leading_asterisk(char* str);
void parse_args(char* str, int nargs, char* args[]);

#endif /*FQGRID_H_INCLUDED*/