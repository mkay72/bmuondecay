#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "sphbes.h"
#include "error_handler.h"
#include "libamos644_interface.h"
#include "fqg_name.h"


void eval_sphbes_amos(enum SPHBES_KIND kind, double l, double complex c, char* name)
{
    fqgrid_t* fqgrid = new_fqgrid(name);
    if (fqgrid->ndim != 1)
    {
        printf("only for rank=1 fqgrid\n");
        delete_fqgrid(fqgrid);
        return;
    }

    qgrid_t* qgrid = fqgrid->qgrid[0];
    int n = qgrid->n;


    double complex *zs = malloc(n * sizeof(double complex));
    // double complex *ys = malloc(n * sizeof(double complex));

    double *xs = qgrid->x;
    for (int i=0; i < n; i++)
    {
        *(zs+i) = *(xs + i) * c;
    }


    switch (kind)
    {
        case SPHBES_J:
            zspbesj_map(l, zs, n, fqgrid->data);
            break;
        case SPHBES_H1:
            zspbesh1_map(l, zs, n, fqgrid->data);
            break;
        default:
            error_handler("in eval_sphbes_amos(%s,*,*,%s); incorrect kind of the bessel function", SPHBES_J, name);
            break;
    }

    free(zs);

    
    if (find_fqgrid_stack(name) != NULL) 
    {
        replace_stack_fqgrid(name, fqgrid);
    }
    else
    {
        push_fqgrid_stack(fqgrid);
    }

    return;

}



void eval_sphbes_amos_fqg_name(enum SPHBES_KIND kind, double l, double complex c, fqg_name_t* fqg_name)
{
    fqg_name_t fqg_name_deep_copy = *fqg_name;

    fqgrid_t* fqg = new_fqgrid_from_fqg_name(&fqg_name_deep_copy);


    qgrid_t* qgrid = fqg->qgrid[0];
    int n = qgrid->n;


    double complex *zs = malloc(n * sizeof(double complex));
    // double complex *ys = malloc(n * sizeof(double complex));

    double *xs = qgrid->x;
    for (int i=0; i < n; i++)
    {
        *(zs+i) = *(xs + i) * c;
    }


    switch (kind)
    {
        case SPHBES_J:
            zspbesj_map(l, zs, n, fqg->data);
            break;
        case SPHBES_H1:
            zspbesh1_map(l, zs, n, fqg->data);
            break;
        default:
            error_handler("in eval_sphbes_amos(%s,*,*,%s); incorrect kind of the bessel function", SPHBES_J, fqg_name_deep_copy.name);
            break;
    }

    free(zs);

    
    if (find_fqgrid_stack(fqg_name_deep_copy.name) != NULL) 
    {
        replace_stack_fqgrid(fqg_name_deep_copy.name, fqg);
    }
    else
    {
        push_fqgrid_stack(fqg);
    }

    return;

}