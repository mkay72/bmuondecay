#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "sphbes.h"
#include "error_handler.h"
#include "libamos644_interface.h"


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
