#include <stdio.h>

#include "libradial_interface.h"
#include "qgrid.h"
#include "fqgrid.h"
#include "fqgrid_stack.h"
#include "name.h"


double eval_dbound_radial(int z, double rms_fm, int knu, double mass, int n, int kappa, char* name)
{
    char name_p1[FQGRID_MAX_NAME];
    char name_m1[FQGRID_MAX_NAME];
    append_suffix(name, "[+1]", name_p1);
    append_suffix(name, "[-1]", name_m1);

    fqgrid_t* fqgrid_p1 = new_fqgrid(name_p1);
    fqgrid_t* fqgrid_m1 = new_fqgrid(name_m1);
    if (fqgrid_p1->ndim != 1)
    {
        printf("only for rank=1 fqgrid\n");
        delete_fqgrid(fqgrid_p1);
        delete_fqgrid(fqgrid_m1);
        return 0.0;
    }

    qgrid_t* qgrid = fqgrid_p1->qgrid[0];
    int nqgrid = qgrid->n;


    double dz = (double)z;
    
    double e_out = 0.0;
    double psi_large[nqgrid];
    double psi_small[nqgrid];

    radial_dbound_(&dz, &rms_fm, &knu, &mass, 
                   &n, &kappa, &nqgrid, qgrid->x, psi_large, psi_small, &e_out);


    for (int i=0; i < nqgrid; i++)
    {
        *(fqgrid_p1->data + i) = psi_large[i];
        *(fqgrid_m1->data + i) = psi_small[i];
    }



    if (find_fqgrid_stack(name_p1) != NULL) 
    {
        replace_stack_fqgrid(name_p1, fqgrid_p1);
    }
    else
    {
        push_fqgrid_stack(fqgrid_p1);
    }

    if (find_fqgrid_stack(name_m1) != NULL) 
    {
        replace_stack_fqgrid(name_m1, fqgrid_m1);
    }
    else
    {
        push_fqgrid_stack(fqgrid_m1);
    }     

    return e_out;
}



double eval_dfree_radial(int z, double rms_fm, int knu, double mass, double e, int kappa, char* name)
{
    char name_p1[FQGRID_MAX_NAME];
    char name_m1[FQGRID_MAX_NAME];
    append_suffix(name, "[+1]", name_p1);
    append_suffix(name, "[-1]", name_m1);

    fqgrid_t* fqgrid_p1 = new_fqgrid(name_p1);
    fqgrid_t* fqgrid_m1 = new_fqgrid(name_m1);
    if (fqgrid_p1->ndim != 1)
    {
        printf("only for rank=1 fqgrid\n");
        delete_fqgrid(fqgrid_p1);
        delete_fqgrid(fqgrid_m1);
        return 0.0;
    }

    qgrid_t* qgrid = fqgrid_p1->qgrid[0];
    int nqgrid = qgrid->n;


    double dz = (double)z;
    
    double phase = 0.0;
    double psi_large[nqgrid];
    double psi_small[nqgrid];

    radial_dfree_(&dz, &rms_fm, &knu, &mass, 
                   &e, &kappa, &nqgrid, qgrid->x, psi_large, psi_small, &phase);


    for (int i=0; i < nqgrid; i++)
    {
        *(fqgrid_p1->data + i) = psi_large[i];
        *(fqgrid_m1->data + i) = psi_small[i];
    }



    if (find_fqgrid_stack(name_p1) != NULL) 
    {
        replace_stack_fqgrid(name_p1, fqgrid_p1);
    }
    else
    {
        push_fqgrid_stack(fqgrid_p1);
    }

    if (find_fqgrid_stack(name_m1) != NULL) 
    {
        replace_stack_fqgrid(name_m1, fqgrid_m1);
    }
    else
    {
        push_fqgrid_stack(fqgrid_m1);
    }     

    return phase;
}