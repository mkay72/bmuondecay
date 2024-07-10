#ifndef SPHBES_H_INCLUDED
#define SPHBES_H_INCLUDED

#include "fqgrid.h"
#include "fqgrid_stack.h"


enum SPHBES_KIND 
{
    SPHBES_J,
    SPHBES_H1
};


void eval_sphbes_amos(enum SPHBES_KIND kind, double l, double complex c, char* name);

void eval_sphbes_amos_fqg_name(enum SPHBES_KIND kind, double l, double complex c, fqg_name_t* fqg_name);


#endif /* SPHBES_H_INCLUDED */