#ifndef NAME_H_INCLUDED
#define NAME_H_INCLUDED


#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "fqgrid.h"

typedef struct
{
    char radical[FQGRID_NAME_RADICAL_MAX];

    char suffix[FQGRID_NAME_SUFFIX_MAX];
    int has_suffix;

    char inflexion[FQGRID_NAME_INFLEXION_MAX];

    char fullname[FQGRID_MAX_NAME];

} name_t;


name_t* new_name(const char* format, ...);

void delete_name(name_t* name);

static void parse_name(name_t* name);

static char* parse_suffix(const char fullname[FQGRID_MAX_NAME], char suffix[FQGRID_NAME_SUFFIX_MAX], int *has_suffix);

static char* parse_inflexion(const char fullname[FQGRID_MAX_NAME], char inflexion[FQGRID_NAME_INFLEXION_MAX], int *has_inflexion);

void modify_radical(name_t *name, const char* format, ...);

void modify_suffix(name_t *name, const char* format, ...);   

static void update_name(name_t* name);

void print_name(name_t* name);

void append_suffix(char name[FQGRID_MAX_NAME], char* s, char name_new[FQGRID_MAX_NAME]);

#endif