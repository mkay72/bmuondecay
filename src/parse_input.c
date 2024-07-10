#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "error_handler.h"
#include "parser.h"
#include "parse_input.h"
#include "constants.h"

static int pushed_back = 0;

static char input_file_name[128];

extern char* yytext;

extern int yylex_destroy(void);


char* yytext_ptr;


static void print_line_by_num(char *file_name, int required_lineno)
{
    FILE *f;
    int c;
    int lineno;

    f = fopen(file_name, "r");
    if (f == NULL) {
//         errquit("print_line_by_num(): cannot open file '%s'", file_name);
    }

    lineno = 1;
    while ((c = fgetc(f)) != EOF) 
    {
        if (lineno == required_lineno)
        {
            printf(" ");
            do 
            {
                putchar(c);
                c = fgetc(f);
            } 
            while (c != '\n' && c != EOF);
            printf("\n");
            fclose(f);
            return;
        }
        if (c == '\n') 
        {
            lineno++;
        }
    }

//     errquit("print_line_by_num(): no line number %d in '%s'", required_lineno, file_name);
    fclose(f);
}



static void yyerror(char *s)
{
    int lineno = yylineno;

    // lineno = yylineno-1 if last token was END_OF_LINE
    if (strcmp(yytext, "\n") == 0) {
        lineno = yylineno - 1;
    }
    fclose(yyin);

    printf("%s:%d:%d: error: %s\n\n", input_file_name, lineno, yycol, s);
    print_line_by_num(input_file_name, lineno);

    // print pointer to the erroneous token
    printf(" ");
    for (int i = 0; i < yycol; i++) 
    {
        printf(" ");
    }
    printf("^\n\n");
    printf("execution terminated.\n");

    exit(1);
}



static int next_token()
{
    if (pushed_back)
    {
        int token_type = pushed_back;
        pushed_back = 0;
        return token_type;
    }
    return yylex();
}



static void put_back(int token_type)
{
    pushed_back = token_type;
}


static int match(int required_type)
{
    int token_type = next_token();
    if (token_type != required_type) 
    {
        return 0;
    }
    return 1;
}


static void str_tolower(char *s)
{
    for (; *s; s++)
    {
        *s = tolower(*s);
    }
}



static void parse_print(options_t* options)
{
    static char *msg = "Incorrect print level.\n"
                       "Possible options are: low, high\n";

    int token_type = next_token();

    if (token_type == TT_WORD) 
    {
        str_tolower(yytext);
        if (strcmp(yytext, "low") == 0) 
        {
            options->print_level = PRINT_LEVEL_LOW;
        }
        else if (strcmp(yytext, "high") == 0)
        {
            options->print_level = PRINT_LEVEL_HIGH;
        }
        else 
        {
            yyerror(msg);
        }
    }
    else 
    {
        yyerror(msg);
    }

}



static void parse_title(char* title)
{
    static char *msg = "wrong argument!\n"
                       "A quoted string is expected";
    int bufsize = 32;

    if (!match(TT_QUOTE))
    {
        yyerror(msg);
    }
    
    size_t len = strlen(yytext) - 2;
    
    strncpy(title, yytext + 1, bufsize - 1);
    title[bufsize - 1] = '\0';
    
    if (len < bufsize - 1) {
        title[len] = '\0';
    }

}



static void parse_rms(options_t* options)
{
    static char *msg = "Incorrect RMS\n"
                       "0.8 < RMS < 10.0 [fm] is expected";
    double rms;
    int token_type;

    token_type = next_token();
    if (token_type != TT_INTEGER && token_type != TT_FLOAT) 
    {
        yyerror(msg);
    }
    
    rms = atof(yytext);
    if ((rms <= 0.8) || rms >= 10.0) 
    {
        yyerror(msg);
    }
    
    options->rms = rms;
}



static void parse_znucl(options_t* options)
{
    static char *msg = "Incorrect input for zcnul\n"
                       "0.0 < znucl < 130.0  is expected";
    double znucl;
    int token_type;

    token_type = next_token();
    if (token_type != TT_INTEGER && token_type != TT_FLOAT) 
    {
        yyerror(msg);
    }
    
    znucl = atof(yytext);
    if ((znucl <= 0.0) || znucl >= 130.0) 
    {
        yyerror(msg);
    }
    
    options->znucl = znucl;
    
}



static void parse_akappa_e(options_t* options)
{
    static char *msg0 = "Incorrect input for akappa_e\n"
                        "akappa_e > 0 is expected";
    
    static char *msg1 = "Incorrect input for akappa_e\n"
                        "integer akappa_e_min is expected";
    
    static char *msg2 = "Incorrect input for akappa_e\n"
                        "integer akappa_e_max is expected";    

    static char *msg3 = "Incorrect input for akappa_e\n"
                        "akappa_e_min <= akappa_e_max is expected";  
    
    static char *msg4 = "Incorrect input for akappa_e\n"
                        "akappa_e > 50 is not supported"; 

    int akappa_e_min;
    int akappa_e_max;
    int token_type;

    token_type = next_token();
    if (token_type != TT_INTEGER) 
    {
        yyerror(msg0);
    }
//     put_back(token_type);
    
    if (token_type != TT_INTEGER) 
    {
        yyerror(msg1);
    }
    akappa_e_min = atoi(yytext);
    
    
    token_type = next_token();
    if (token_type != TT_INTEGER) 
    {
        yyerror(msg2);
    }
    akappa_e_max = atof(yytext);    
    
    if (akappa_e_max < akappa_e_min)
    {
        yyerror(msg3);
    }

    if ((akappa_e_max > 50) || (akappa_e_min > 50))
    {
        yyerror(msg4);
    }    
    
    options->akappa_e_min = akappa_e_min;
    options->akappa_e_max = akappa_e_max;
}



static void parse_energy_e(options_t* options)
{
    static char *msg1 = "Incorrect input for energy_e\n"
                        "energy_e_min > m_e = 0.510998950 [MeV] is expected";
    
    static char *msg2 = "Incorrect input for energy_e\n"
                        "energy_e_max < 105 [MeV] is expected";
      
    static char *msg3 = "Incorrect input for energy_e\n"
                        "energy_e_min < energy_e_max is expected";  
    
    double energy_e_min;
    double energy_e_max;
    int token_type;

    token_type = next_token();
    if (token_type != TT_INTEGER && token_type != TT_FLOAT) 
    {
        yyerror(msg1);
    }
    
    energy_e_min = atof(yytext);
    if (energy_e_min < me_MeV)
    {
        yyerror(msg1);
    }
    
    
    token_type = next_token();
    if (token_type != TT_INTEGER && token_type != TT_FLOAT) 
    {
        yyerror(msg2);
    }
    
    energy_e_max = atof(yytext);    
    if (energy_e_max > 105.0) 
    {
        yyerror(msg2);
    }
    
    
    if (energy_e_max <= energy_e_min)
    {
        yyerror(msg3);
    }
    
    options->energy_e_min = energy_e_min;
    options->energy_e_max = energy_e_max;
}



static void parse_rbox(options_t* options)
{
    static char *msg1 = "Incorrect input for rbox\n"
                        "r1 > 1e-6 [ru] is expected";
    
    static char *msg2 = "Incorrect input for rbox\n"
                        "r2 < 100 [ru] is expected";
      
    static char *msg3 = "Incorrect input for energy_e\n"
                        "r1 < r2 is expected";  
    
    double r1;
    double r2;
    int token_type;

    token_type = next_token();
    if (token_type != TT_INTEGER && token_type != TT_FLOAT) 
    {
        yyerror(msg1);
    }
    
    r1 = atof(yytext);
    
    
    token_type = next_token();
    if (token_type != TT_INTEGER && token_type != TT_FLOAT) 
    {
        yyerror(msg2);
    }
    r2 = atof(yytext);    
    
    
    if (r2 <= r1)
    {
        yyerror(msg3);
    }
    
    options->r1 = r1;
    options->r2 = r2;
}



static void parse_nquad_r(options_t* options)
{
    static char *msg1 = "Incorrect input for nquad_r\n"
                        "nsub_intervals > 0 is expected";
    
    static char *msg2 = "Incorrect input for nquad_r\n"
                        "nnodes_per_interval > 1 is expected";
    
    int nsub_r;
    int nqnode_r;
    int token_type;

    token_type = next_token();
    if (token_type != TT_INTEGER) 
    {
        yyerror(msg1);
    }
    
    nsub_r = atoi(yytext);
    
    
    token_type = next_token();
    if (token_type != TT_INTEGER) 
    {
        yyerror(msg2);
    }
    nqnode_r = atoi(yytext);    
    
    
    options->nsub_r = nsub_r;
    options->nqnode_r = nqnode_r;
}



static void parse_nquad_k(options_t* options)
{
    static char *msg1 = "Incorrect input for nquad_k\n"
                        "nsub_intervals > 0 is expected";
    
    static char *msg2 = "Incorrect input for nquad_k\n"
                        "nnodes_per_interval > 1 is expected";
    
    int nsub_k;
    int nqnode_k;
    int token_type;

    token_type = next_token();
    if (token_type != TT_INTEGER) 
    {
        yyerror(msg1);
    }
    
    nsub_k = atoi(yytext);
    
    
    token_type = next_token();
    if (token_type != TT_INTEGER) 
    {
        yyerror(msg2);
    }
    nqnode_k = atoi(yytext);    
    
    
    options->nsub_k = nsub_k;
    options->nqnode_k = nqnode_k;
}



static void parse_nquad_e(options_t* options)
{
    static char *msg1 = "Incorrect input for nquad_e\n"
                        "nsub_intervals > 0 is expected";
    
    static char *msg2 = "Incorrect input for nquad_e\n"
                        "nnodes_per_interval > 1 is expected";
    
    int nsub_e;
    int nqnode_e;
    int token_type;

    token_type = next_token();
    if (token_type != TT_INTEGER) 
    {
        yyerror(msg1);
    }
    
    nsub_e = atoi(yytext);
    
    
    token_type = next_token();
    if (token_type != TT_INTEGER) 
    {
        yyerror(msg2);
    }
    nqnode_e = atoi(yytext);    
    
    
    options->nsub_e = nsub_e;
    options->nqnode_e = nqnode_e;
}



static void parse_muon_quantum_numbers(options_t* options)
{
    static char *msg1 = "Incorrect input for muon\n"
                        "n_mu >= 1 is expected";
    
    static char *msg2 = "Incorrect input for muon\n"
                        "kappa_mu integer is expected";
    
    int n_muon;
    int kappa_muon;
    int token_type;

    token_type = next_token();
    if (token_type != TT_INTEGER) 
    {
        yyerror(msg1);
    }
    
    n_muon = atoi(yytext);
    
    
    token_type = next_token();
    if (token_type != TT_INTEGER && token_type != TT_FLOAT) 
    {
        yyerror(msg2);
    }
    kappa_muon = (int) (atof(yytext));
    
    
    options->n_muon = n_muon;
    options->kappa_muon = kappa_muon;
}


static void parse_nuclear_model(options_t* options)
{
    static char *msg1 = "Incorrect input for nuclear model\n"
                        "1 : point-like (Coulomb)\n"
                        "2 : Fermi\n";
        
    int inuclear_model;
    int token_type;

    token_type = next_token();
    if (token_type != TT_INTEGER) 
    {
        yyerror(msg1);
    }
    
    inuclear_model = atoi(yytext);

    if ((inuclear_model < 1) || (inuclear_model > 2))
    {
        yyerror(msg1);
    }
    
    options->inuclear_model = inuclear_model;
}


void set_input_file_name(char *file_name)
{
    strcpy(input_file_name, file_name);
}


/**
 * echo some file to some another file; returns 0 if OK, 1 if error
 */
int echo(char *file_name, FILE *dst)
{
    FILE *f;
    char line[MAX_LINE_LEN];

    f = fopen(file_name, "r");
    if (f == NULL) 
    {
        return 1;
    }

    while (fgets(line, MAX_LINE_LEN, f) != NULL) 
    {
        fprintf(dst, " %s", line);
    }

    fclose(f);

    return 0;
}


int parse_input(char *file_name, options_t* options)
{
    // printf("options = %p\n", options);
    int token_type;

    yyin = fopen(file_name, "r");
    if (yyin == NULL) 
    {
        error_handler("Input file \"%s\" not found", file_name);
    }

    printf("Reading input file: %s\n\n", file_name);
    printf("\n");
    printf("\t****************\n");
    printf("\t** Input file **\n");
    printf("\t****************\n\n");
    printf(" -----\n");
    echo(file_name, stdout);
    printf(" -----\n");
    printf("\n\n");    


    set_input_file_name(file_name);


    token_type = next_token();
    while (token_type != END_OF_FILE) 
    {
        switch (token_type)
        {
            case KEY_PRINT:
                parse_print(options);
                break;
            case KEY_RMS:
                parse_rms(options);
                break;
            case KEY_ZNUCL:
                parse_znucl(options);
                break;
            case KEY_NUCLEAR_MODEL:
                parse_nuclear_model(options);
                break;                
            case KEY_AKAPPA_E:
                parse_akappa_e(options);
                break;        
            case KEY_ENERGY_E:
                parse_energy_e(options);
                break;
            case KEY_RBOX:
                parse_rbox(options);
                break;
            case KEY_NQUAD_R:
                parse_nquad_r(options);
                break;                
            case KEY_NQUAD_K:
                parse_nquad_k(options);
                break;  
            case KEY_NQUAD_E:
                parse_nquad_e(options);
                break;
            case KEY_MUON_QUANTUM_NUMBERS:
                parse_muon_quantum_numbers(options);
                break;
            case END_OF_LINE:
                break;                
            default:
                yyerror("unknown keyword");
                break;
        }


        if (token_type != END_OF_LINE && token_type != END_OF_FILE) 
        {
            token_type = next_token();
            if (token_type != END_OF_LINE && token_type != END_OF_FILE) 
            {
                yyerror("end of line is expected");
            }
        }            
        
        token_type = next_token();
    }
    
    fclose(yyin);
    yylex_destroy();

}
