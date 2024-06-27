#include <string.h>

#include "fqgrid_stack.h"
#include "fqgrid.h"
#include "qgrid.h"
#include "error_handler.h"
#include "multptws.h"
#include "constants.h"



void multptws(char* name_fqgrid_1, char* name_fqgrid_2, char* target)
{
    fqgrid_t* fqg1;
    fqgrid_t* fqg2;

    fqgrid_t* fqg1fqg2;


    fqg1 = find_fqgrid_stack(name_fqgrid_1);
    if (fqg1 == NULL) 
    {
        error_handler("in multptws(), fqgrid '%s' is not found in the stack", name_fqgrid_1);
    }      
    fqg2 = find_fqgrid_stack(name_fqgrid_2);
    if (fqg2 == NULL) 
    {
        error_handler("in multptws(), fqgrid '%s' is not found in the stack", name_fqgrid_2);
    }

    fqg1fqg2 = multptws_fqg(fqg1, fqg2);
    strcpy(fqg1fqg2->name, target);

    if (find_fqgrid_stack(target) != NULL) 
    {
        replace_stack_fqgrid(target, fqg1fqg2);
    }
    else
    {
        push_fqgrid_stack(fqg1fqg2);
    }
    
    return;    

}



void scale_i(double complex a, char* name_fqgrid_1)
{
    fqgrid_t* fqg1;

    fqg1 = find_fqgrid_stack(name_fqgrid_1);
    if (fqg1 == NULL) 
    {
        error_handler("in scale_i(), fqgrid '%s' is not found in the stack", name_fqgrid_1);
    }      

    scale_i_fqg(a, fqg1);
    
    return;    

}


void scale_i_fqg(double complex a, fqgrid_t* fqg)
{
    int n = fqg->size_tot;
    cblas_zscal(n, &a, fqg->data, 1);
    
    return;
}


// returns vector norm with p=1 of a given fqg
// res = sum_i |a_i|
double vecnorm_L1(char* name_fqg)
{
    fqgrid_t *fqg;
    fqg = find_fqgrid_stack(name_fqg);
    if (fqg == NULL) 
    {
        error_handler("in abssum(), fqgrid '%s' is not found in the stack", name_fqg);
    }      

    return vecnorm_L1_fqg(fqg);

}


double vecnorm_L1_fqg(fqgrid_t* fqg)
{
    int n = fqg->size_tot;
    return cblas_dzasum(n, fqg->data, 1);
    
}





fqgrid_t* multptws_fqg(fqgrid_t* fqg1, fqgrid_t* fqg2)
{
    multptws_fqg_is_valid(fqg1, fqg2);
    
    enum MULTPTWS_FQG_OPERATION_TYPE operation_type = multptws_fqg_select_operation_type(fqg1, fqg2);
    fqgrid_t* fqg1fqg2 = multptws_fqg_create_template(fqg1, fqg2, operation_type);

    switch (operation_type)
    {
    case VV:
        multptws_fqgrid_type_vv_gg(fqg1, fqg2, fqg1fqg2);
        break;
    case VG:
        multptws_fqgrid_type_vg(fqg1, fqg2, fqg1fqg2);
        break;
    case GV:
        multptws_fqgrid_type_gv(fqg1, fqg2, fqg1fqg2);
        break;
    case GG:
        multptws_fqgrid_type_vv_gg(fqg1, fqg2, fqg1fqg2);
    default:
        break;
    }

    return fqg1fqg2;

}

// v(x) * h(x) or a(x,y) * b(x,y)
// pointwise multiplication of two arrays
void multptws_fqgrid_type_vv_gg(fqgrid_t* fqg1, fqgrid_t* fqg2, fqgrid_t* fqg1fqg2)
{
    int n = fqg1->size_tot;

    for (int i=0; i < n; i++)
    {    
        fqg1fqg2->data[i] = fqg1->data[i] * fqg2->data[i];
    }

    return;
}

// v(x) * g(x,y)
// scales rows of g(x,y) by elements of v(x)
void multptws_fqgrid_type_vg(fqgrid_t* fqg1, fqgrid_t* fqg2, fqgrid_t* fqg1fqg2)
{
    int n = fqg1->size[0];
    int m = fqg2->size[1];

    double complex vx = 0.0;
    for (int i=0; i < n; i++)
    {
        vx = *(fqg1->data + i);
        cblas_zaxpy(m, &vx, fqg2->data + i*m, 1, fqg1fqg2->data + i*m, 1);
    }

    return;
}

// g(x,y) * v(y)
// scales columns of g(x,y) by elements of v(y)
void multptws_fqgrid_type_gv(fqgrid_t* fqg1, fqgrid_t* fqg2 , fqgrid_t* fqg1fqg2)
{
    int n = fqg1->size[0];
    int m = fqg2->size[0];

    double complex vy = 0.0;
    for (int i=0; i < m; i++)
    {
        vy = *(fqg2->data + i);
        cblas_zaxpy(n, &vy, fqg1->data + i, m, fqg1fqg2->data + i, m);
    }

    return;
}


void multptws_fqg_is_valid(fqgrid_t* fqg1, fqgrid_t* fqg2)
{
    int is_valid = 0;

    if (fqg1->ndim == fqg2->ndim)
    {
        if (fqg1->ndim == 1)
        {
            // f(x) * g(x) 
            if (fqg1->qgrid[0] == fqg2->qgrid[0])
            {
                is_valid = 1;
            }
        }
        else if (fqg1->ndim == 2)
        {
            // a(x,y) * b(x,y)
            if ((fqg1->qgrid[0] == fqg2->qgrid[0]) && (fqg1->qgrid[1] == fqg2->qgrid[1]))
            {
                is_valid = 1;
            }            
        }
    }
    // dimensions are different
    else if ((fqg1->ndim == 2) && (fqg2->ndim == 1))
    {
        // a(x,y) * f(y)
        if (fqg1->qgrid[1] == fqg2->qgrid[0])
        {
            is_valid = 1;
        }
    }
    else if ((fqg1->ndim == 1) && (fqg2->ndim == 2))
    {
        // f(x) * a(x,y)
        if (fqg1->qgrid[0] == fqg2->qgrid[0])
        {
            is_valid = 1;
        }        
    }

    if (is_valid == 0)
    {
        error_handler("in multptws_fqg(%s, %s); invalid operation", fqg1->name, fqg2->name);
    }

    return;

}



enum MULTPTWS_FQG_OPERATION_TYPE multptws_fqg_select_operation_type(fqgrid_t* fqg1, fqgrid_t* fqg2)
{
    if ((fqg1->ndim == 1) && (fqg2->ndim == 1))
    {
        return VV;
    }
    else if ((fqg1->ndim == 1) && (fqg2->ndim == 2))
    {
        return VG;
    }
    else if ((fqg1->ndim == 2) && (fqg2->ndim == 1))
    {
        return GV;
    }
    else if ((fqg1->ndim == 2) && (fqg2->ndim == 2))
    {
        return GG;
    }
    error_handler("in multptws_fqg_select_operation_type(%s, %s); incorrect type of operation", fqg1->name, fqg2->name);

}



fqgrid_t* multptws_fqg_create_template(fqgrid_t* fqgrid_g1, fqgrid_t* fqgrid_g2, enum MULTPTWS_FQG_OPERATION_TYPE type)
{
    char temp_name[FQGRID_MAX_NAME];

    char qgrid_g1[QGRID_MAX_NAME];
    char qgrid_g2[QGRID_MAX_NAME];

    switch (type)
    {
    case VV:
        strcpy(qgrid_g1, fqgrid_g1->qgrid[0]->name);
        strcpy(temp_name, "multptws_fqg_template");
        strcat(temp_name, "(");
        strcat(temp_name, qgrid_g1);
        strcat(temp_name, ")");
        break;    
    case GV:
        strcpy(qgrid_g1, fqgrid_g1->qgrid[0]->name);
        strcpy(qgrid_g2, fqgrid_g1->qgrid[1]->name);

        strcpy(temp_name, "multptws_fqg_template");
        strcat(temp_name, "(");
        strcat(temp_name, qgrid_g1);
        strcat(temp_name, ",");
        strcat(temp_name, qgrid_g2);
        strcat(temp_name, ")");    
        break;
    case VG:
        strcpy(qgrid_g1, fqgrid_g2->qgrid[0]->name);
        strcpy(qgrid_g2, fqgrid_g2->qgrid[1]->name);

        strcpy(temp_name, "multptws_fqg_template");
        strcat(temp_name, "(");
        strcat(temp_name, qgrid_g1);
        strcat(temp_name, ",");
        strcat(temp_name, qgrid_g2);
        strcat(temp_name, ")");    
        break;
    case GG:
        strcpy(qgrid_g1, fqgrid_g1->qgrid[0]->name);
        strcpy(qgrid_g2, fqgrid_g2->qgrid[1]->name);

        strcpy(temp_name, "multptws_fqg_template");
        strcat(temp_name, "(");
        strcat(temp_name, qgrid_g1);
        strcat(temp_name, ",");
        strcat(temp_name, qgrid_g2);
        strcat(temp_name, ")");    
        break;        
    }

    fqgrid_t* temp = new_fqgrid(temp_name);

    return temp;
}