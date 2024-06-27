%{
#include "parser.h"

#define MAX_TOKEN_LEN 1024


void inc_col();
int yycol = 0;
int prev_len = -1;

%}

%option case-insensitive


%% 
"#"[^\n]*       { inc_col(); } /* single-line comments beginning with '#' */
[ \t\r]         { inc_col(); } /* ignore whitespace */
print           { inc_col(); return KEY_PRINT;            }
rms             { inc_col(); return KEY_RMS;              }
znucl           { inc_col(); return KEY_ZNUCL;            }
akappa_e        { inc_col(); return KEY_AKAPPA_E;         }
energy_e        { inc_col(); return KEY_ENERGY_E;         }
rbox            { inc_col(); return KEY_RBOX;             }
nquad_r         { inc_col(); return KEY_NQUAD_R;          }
nquad_k         { inc_col(); return KEY_NQUAD_K;          }
nquad_e         { inc_col(); return KEY_NQUAD_E;          }
muon            { inc_col(); return KEY_MUON_QUANTUM_NUMBERS;          }
nuclmodel       { inc_col(); return KEY_NUCLEAR_MODEL;    }
\".*\"          { inc_col(); return TT_QUOTE;             }
[0-9]*          { inc_col(); return TT_INTEGER;           }
[+-]?[0-9]*(\.[0-9]*)?([eE][+-]?[0-9]+)?   { inc_col(); return TT_FLOAT; }
([^ *\t\n\[\]])+ { inc_col(); return TT_WORD;     }
\n              { inc_col(); yylineno++; return END_OF_LINE; }
<<EOF>>         { return END_OF_FILE; }
%%


/* just dummy */
int yywrap() 
{
   return 1;
}


/**
 * updates the number of the current column
 */
void inc_col()
{
    // update yycol
    if (prev_len == -1) 
    {
        yycol = 0;
    }
    else 
    {
        yycol += prev_len;
    }

    // update prev_len
    if (strcmp(yytext, "\n") == 0) 
    {
        prev_len = -1;
    }
    else 
    {
        prev_len = strlen(yytext);
    }
}
