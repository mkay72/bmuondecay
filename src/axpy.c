#include <string.h>

#include "fqgrid_stack.h"
#include "qgrid.h"
#include "error_handler.h"
#include "axpy.h"


void axpy(double complex a, char* name1, char* name2, char* target)
{
    fqgrid_t* fqgrid_x;
    fqgrid_t* fqgrid_y;
    fqgrid_t* fqgrid_axpy;

    fqgrid_x = find_fqgrid_stack(name1);
    if (fqgrid_x == NULL) {
        error_handler("in axpy(), fqgrid '%s' is not found in the stack", name1);
    }    
    fqgrid_y = find_fqgrid_stack(name2);
    if (fqgrid_y == NULL) {
        error_handler("in axpy(), fqgrid '%s' is not found in the stack", name2);
    }     

    fqgrid_axpy = axpy_fqgrid(a, fqgrid_x, fqgrid_y);
    strcpy(fqgrid_axpy->name, target);    

    if (find_fqgrid_stack(target) != NULL) 
    {
        replace_stack_fqgrid(target, fqgrid_axpy);
    }
    else
    {
        push_fqgrid_stack(fqgrid_axpy);
    }
    
    return;

}


fqgrid_t* axpy_fqgrid(double complex a, fqgrid_t* fqgrid_x, fqgrid_t* fqgrid_y)
{

    char name[FQGRID_MAX_NAME];
    if (fqgrid_x->ndim != fqgrid_y->ndim)
    {
        error_handler("in axpy_fqgrid, %s and %s have different ndim", fqgrid_x->name, fqgrid_y->name);
    }

    for (int idim=0; idim < fqgrid_x->ndim; idim++)
    {
        if (fqgrid_x->qgrid[idim] != fqgrid_y->qgrid[idim])
        {
            error_handler("in axpy_fqgrid, %s and %s are defined on differend qgrids", fqgrid_x->name, fqgrid_y->name);
        }
    }

    char grid_name1[QGRID_MAX_NAME];
    strcpy(grid_name1, fqgrid_x->qgrid[0]->name);
    strcpy(name, "aux");


    int n = 0;
    if (fqgrid_x->ndim == 1)
    {
        strcat(name, "(");
        strcat(name, grid_name1);
        strcat(name, ")");
        n = fqgrid_x->size[0];
    }
    else if (fqgrid_x->ndim == 2)
    {
        char grid_name2[QGRID_MAX_NAME];
        strcpy(grid_name2, fqgrid_x->qgrid[1]->name);
        strcat(name, "(");
        strcat(name, grid_name1);
        strcat(name, ",");
        strcat(name, grid_name2);
        strcat(name, ")");
        n = fqgrid_x->size[0] * fqgrid_x->size[1];
    }
    else
    {
        error_handler("in axpy_fqgrid, ndim = %d is not supported", fqgrid_x->ndim);
    }


    fqgrid_t* fqgrid_axpy = new_fqgrid(name);

    
    double complex *p_axpy = fqgrid_axpy->data;
    double complex *p_x = fqgrid_x->data;
    double complex *p_y = fqgrid_y->data;

    for (int i=0; i < n; i++)
    {
        *(p_axpy +i) = a *(*(p_x+i)) + *(p_y+i);
    }


    return fqgrid_axpy;
}


