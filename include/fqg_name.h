#ifndef FQG_NAME_H_INCLUDED
#define FQG_NAME_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "fqg_name_token_table.h"

#define MAX_STEM_LEN 32
#define MAX_SUFFIX_LEN 32
#define MAX_SYS_SUFFIX_LEN 16
#define MAX_INFLEXION_LEN 8

typedef struct
{
    int type;
    char stem[MAX_STEM_LEN];
    char suffix[MAX_SUFFIX_LEN];
    char sys_suffix[MAX_SYS_SUFFIX_LEN];
    char inflexion[2][MAX_INFLEXION_LEN];
    char name[MAX_STEM_LEN+MAX_SUFFIX_LEN+MAX_SYS_SUFFIX_LEN+2*MAX_INFLEXION_LEN];
} fqg_name_t;

static void fqg_name_set_type_zero(fqg_name_t* fqg_name);

static void fqg_name_set_stem_zero(fqg_name_t* fqg_name);

static void fqg_name_set_suffix_zero(fqg_name_t* fqg_name);

static void fqg_name_set_sys_suffix_zero(fqg_name_t* fqg_name);

static void fqg_name_set_inflexion_zero(fqg_name_t* fqg_name);

static void fqg_name_set_name_zero(fqg_name_t* fqg_name);

void fqg_name_set_zero(fqg_name_t* fqg_name);

static void fqg_name_parse_type(fqg_name_t* fqg_name);

static void fqg_name_parse_stem(fqg_name_t* fqg_name);

static void fqg_name_parse_suffix(fqg_name_t* fqg_name);

static void fqg_name_parse_sys_suffix(fqg_name_t* fqg_name);

static void fqg_name_parse_inflexion(fqg_name_t* fqg_name);

void fqg_name_parse_from_string(char* name_string, fqg_name_t* fqg_name);

void update_fqg_name(fqg_name_t* fqg_name);

void set_fqg_type(fqg_name_t* fqg_name, int type);

void set_fqg_stem(fqg_name_t* fqg_name, char* stem);

void set_fqg_suffix(fqg_name_t* fqg_name, const char* format, ...);

void set_fqg_sys_suffix(fqg_name_t* fqg_name, const char* format, ...);

void set_fqg_inflexion(fqg_name_t* fqg_name, char* inflexion[2]);

fqg_name_t* new_fqg_name(char* name_string);

void delete_fqg_name(fqg_name_t* fqg_name);

void print_fqg_name(fqg_name_t* fqg_name);


#endif