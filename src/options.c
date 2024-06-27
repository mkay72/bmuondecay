#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "options.h"


options_t* options = NULL;

options_t* new_options(void)
{
    options_t *options = (options_t*) malloc(sizeof(options_t));
    
    options->r1 = 1e-6;
    options->r2 = 2.0;
    
    options->znucl = 1;
    options->rms = 0.0;
    options->inuclear_model = 1;

    options->n_muon = 1;
    options->kappa_muon = -1;
    
    options->akappa_e_min = 1;
    options->akappa_e_max = 1;
    options->energy_e_min = 1.0;
    options->energy_e_max = 100.0;
    
    options->nsub_r = 0;
    options->nqnode_r = 12;

    options->nsub_k = 0;
    options->nqnode_k = 12;

    options->nsub_e = 0;
    options->nqnode_e = 12;    

    options->print_level = PRINT_LEVEL_LOW;
    
    return options;
}


void delete_options(options_t* options)
{
    if (options != NULL)
    {
        free(options);
    }
}

void print_options(options_t* options)
{
    if (options != NULL)
    {
        printf("\n");
        printf("\t*************\n");
        printf("\t** Options **\n");
        printf("\t*************\n\n");
        
        printf(" %-14s = %2d\n", "znucl", options->znucl);
        printf(" %-14s = %2d\n", "nuclear_model", options->inuclear_model);
        printf(" %-14s = %7.4f [fm]\n", "rms", options->rms);
        printf(" %-14s = %2d\n", "n_muon", options->n_muon);
        printf(" %-14s = %+2d\n", "kappa_muon", options->kappa_muon);
        printf(" %-14s = %2d %2d\n", "akappa_e", options->akappa_e_min, options->akappa_e_max);
        printf(" %-14s = (%4.2f, %4.2f) [MeV]\n", "energy_e", options->energy_e_min, options->energy_e_max);
        printf(" %-14s = (%4.2e, %4.2e) [ru]\n", "rbox", options->r1, options->r2);        
        printf(" %-14s = %2d %2d\n", "nquad_r", options->nsub_r, options->nqnode_r);
        printf(" %-14s = %2d %2d\n", "nquad_k", options->nsub_k, options->nqnode_k);
        printf(" %-14s = %2d %2d\n", "nquad_e", options->nsub_e, options->nqnode_e);
        printf(" %-14s = %2d\n", "print level", options->print_level);
    }
}