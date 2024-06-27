#ifndef PARSE_INPUT_H_INCLUDED
#define PARSE_INPUT_H_INCLUDED

#define MAX_LINE_LEN 1024

#include "parser.h"
#include "options.h"

// extern char* yytext;

// extern char* yytext_ptr;

static void print_line_by_num(char *file_name, int required_lineno);

static void yyerror(char *s);

static int next_token();

static void put_back(int token_type);

static int match(int required_type);

static void str_tolower(char *s);

static void parse_title(char* title);

static void parse_print(options_t* options);

static void parse_nuclear_model(options_t* options);

static void parse_rms(options_t* options);

static void parse_znucl(options_t* options);

static void parse_akappa_e(options_t* options);

static void parse_energy_e(options_t* options);

static void parse_rbox(options_t* options);

static void parse_nquad_r(options_t* options);

static void parse_nquad_k(options_t* options);

static void parse_nquad_e(options_t* options);

static void parse_muon_quantum_numbers(options_t* options);


static void set_input_file_name(char *file_name);

int parse_input(char *file_name, options_t* options);

#endif /* PARSE_INPUT_H_INCLUDED */