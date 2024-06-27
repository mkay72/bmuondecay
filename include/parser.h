#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <stdio.h>

extern FILE *yyin;
extern char *yytext;
extern char *yytext_ptr;
extern int yylineno;
extern int yycol;

int yylex();

enum {
    /* token types */
    TT_QUOTE = 0,
    TT_INTEGER,
    TT_FLOAT,
    TT_WORD,
    /* keys */
    KEY_TITLE,
    KEY_PRINT,
    KEY_RMS,
    KEY_ZNUCL,
    KEY_AKAPPA_E,
    KEY_ENERGY_E,
    KEY_RBOX,
    KEY_NQUAD_R,
    KEY_NQUAD_K,
    KEY_NQUAD_E,
    KEY_MUON_QUANTUM_NUMBERS,
    KEY_NUCLEAR_MODEL,
    /* special */
    END_OF_LINE,
    END_OF_FILE
};

#endif /* PARSER_H_INCLUDED */
 
