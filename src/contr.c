#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "fqgrid_stack.h"
#include "fqgrid.h"
#include "qgrid.h"
#include "error_handler.h"
#include "contr.h"
#include "constants.h"
#include <complex.h>




void contr_gvg(char* name_g1, enum CONCAT_CONJ is_conj, char* name_v, char* name_g2, char* target)
{
    fqgrid_t* g1;
    fqgrid_t* g2;
    fqgrid_t* v;

    fqgrid_t* g1vg2;


    g1 = find_fqgrid_stack(name_g1);
    if (g1 == NULL) 
    {
        error_handler("in concat(), fqgrid '%s' is not found in the stack", name_g1);
    }    
    v = find_fqgrid_stack(name_v);
    if (v == NULL) 
    {
        error_handler("in concat(), fqgrid '%s' is not found in the stack", name_v);
    }      
    g2 = find_fqgrid_stack(name_g2);
    if (g2 == NULL) 
    {
        error_handler("in concat(), fqgrid '%s' is not found in the stack", name_g2);
    }     

    g1vg2 = contr_gvg_fqg(g1, is_conj, v, g2);
    strcpy(g1vg2->name, target);

    if (find_fqgrid_stack(target) != NULL) 
    {
        replace_stack_fqgrid(target, g1vg2);
    }
    else
    {
        push_fqgrid_stack(g1vg2);
    }
    
    return;

}


void contr_gvg_fqg_is_valid(fqgrid_t* fqg1, fqgrid_t* fqgv, fqgrid_t* fqg2)
{
    int is_valid = 0;

    if ((fqg1->ndim == 2) && (fqg2->ndim == 2) && (fqgv->ndim == 1))
    {
        if ((fqg1->qgrid[1] == fqg2->qgrid[0]) && (fqg1->qgrid[1] == fqgv->qgrid[0]))
        {
            is_valid = 1;
        }
        else
        {
            error_handler("in contr_gvg(%s, %s, %s); the grids are not consisntent with the oparation", fqg1->name, fqgv->name, fqg2->name);
        }
    }
    else
    {
        error_handler("in contr_gvg(%s, %s, %s); the dimensions of the operands are not consisntent with the oparation", fqg1->name, fqgv->name, fqg2->name);
    }
    if (is_valid == 0)
    {
        error_handler("in contr_gvg(%s, %s, %s); invalid operation", fqg1->name, fqgv->name, fqg2->name);
    }
}



fqgrid_t* contr_gvg_fqg(fqgrid_t* fqg1, enum CONCAT_CONJ is_conj, fqgrid_t* fqgv, fqgrid_t* fqg2)
{
    contr_gvg_fqg_is_valid(fqg1, fqgv, fqg2);

    fqgrid_t* g1vg2 = contr_fqgrid_template(fqg1, fqg2);

    // g1(m,k)
    // v(k)
    // g2(k,n)
    int m = fqg1->size[0];
    int n = fqg2->size[1];
    int k = fqg2->size[0];

    // cblas_zgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, &ZONE, fqg1->data, k, fqg2->data, n, &ZZERO, g1vg2->data, n);    

    double complex *g1_scaled = malloc(m * k * sizeof(double complex));

    cblas_zcopy(m*k, fqg1->data, 1, g1_scaled, 1);

    int mk = m*k;
    switch(is_conj)
    {
        case CONJ:
            for (int i=0; i < mk; i++)
            {
                *(g1_scaled + i) = conj(*(g1_scaled + i));
            }
            break;
        case NO_CONJ:
            break;
    }    


    for (int i=0; i < k; i++)
    {
        double complex vi = *(fqgv->data + i);
        double wi = *(fqgv->qgrid[0]->w + i);
        double complex alpha = vi*wi;
        // printf("(%f, %f)\t%f\t%f\n", vi, wi, alpha);
        
        // g1_scaled <--- diag(w) * diag(v) * g1
        cblas_zscal(m, &alpha, g1_scaled + i, k);
            
    }
    cblas_zgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, &ZONE, g1_scaled, k, fqg2->data, n, &ZZERO, g1vg2->data, n);    
    
    free(g1_scaled);

    return g1vg2;
}



fqgrid_t* contr_fqgrid_template(fqgrid_t* fqg1, fqgrid_t* fqg2)
{
    char temp_name[FQGRID_MAX_NAME];

    char qgrid_g1[QGRID_MAX_NAME];
    char qgrid_g2[QGRID_MAX_NAME];

    strcpy(qgrid_g1, fqg1->qgrid[0]->name);
    strcpy(qgrid_g2, fqg2->qgrid[1]->name);

    strcpy(temp_name, "contr_fqgrid_temp");
    strcat(temp_name, "(");
    strcat(temp_name, qgrid_g1);
    strcat(temp_name, ",");
    strcat(temp_name, qgrid_g2);
    strcat(temp_name, ")");


    fqgrid_t* temp = new_fqgrid(temp_name);
    return temp;
}



double complex contr_vgv(char* name_v1, enum CONCAT_CONJ is_conj, char* name_g, char* name_v2)
{
    fqgrid_t* v1;
    fqgrid_t* v2;
    fqgrid_t* g;

    double complex v1gv2 = CMPLX(0.0, 0.0);


    v1 = find_fqgrid_stack(name_v1);
    if (v1 == NULL) 
    {
        error_handler("in contr_vgv(), fqgrid '%s' is not found in the stack", name_v1);
    }    
    g = find_fqgrid_stack(name_g);
    if (g == NULL) 
    {
        error_handler("in contr_vgv(), fqgrid '%s' is not found in the stack", name_g);
    }      
    v2 = find_fqgrid_stack(name_v2);
    if (v2 == NULL) 
    {
        error_handler("in contr_vgv(), fqgrid '%s' is not found in the stack", name_v2);
    }     

    v1gv2 = contr_fqgrid_vgv(v1, is_conj, g, v2);
    // printf("v1gv2 = %e %e\n", v1gv2);
    
    return v1gv2;

}


double complex contr_fqgrid_vgv(fqgrid_t* v1, enum CONCAT_CONJ is_conj, fqgrid_t* g, fqgrid_t* v2)
{
    // g(m,n)
    // v1(m)
    // v2(n)
    int m = v1->size_tot;
    int n = v2->size_tot;

    double complex* v2w = (double complex*) malloc(n * sizeof(double complex));


    double* w2 = v2->qgrid[0]->w;
    for (int i=0; i < n; i++)
    {
        double complex v2i = *(v2->data+i);
        double wi = *(w2+i);
        *(v2w+i) =  v2i*wi;
    }


    double complex *v1w = (double complex*) malloc(m * sizeof(double complex));
        
    double* w1 = v1->qgrid[0]->w;


    switch (is_conj)
    {
    case CONJ:
        for (int i=0; i < m; i++)
        {
            double complex v1i = conj(*(v1->data+i));
            double wi = *(w1+i);
            *(v1w+i) = v1i*wi;

        }
        break;
    
    case NO_CONJ:
        for (int i=0; i < m; i++)
        {
            double complex v1i = *(v1->data+i);
            double wi = *(w1+i);
            *(v1w+i) = v1i*wi;
        }    
        break;
    }

    double complex *gv2 = (double complex*) malloc(n*m * sizeof(double complex));

    cblas_zgemv(CblasRowMajor, CblasNoTrans, m, n, &ZONE, g->data, n, v2w, 1, &ZZERO, gv2, 1);
    // https://stackoverflow.com/questions/64005454/gemv-blas-and-cublas-missing-the-conjugate-only-ordering-option

    double complex v1gv2;
    cblas_zdotu_sub(m, v1w, 1, gv2, 1, &v1gv2);
    
    free(v2w);
    free(v1w);
    free(gv2);

    return v1gv2;

}


double complex contr_vdiaggv(char* name_v1, enum CONCAT_CONJ is_conj, char* name_g, char* name_v2)
{
    fqgrid_t* v1;
    fqgrid_t* v2;
    fqgrid_t* g;

    double complex v1gv2 = CMPLX(0.0, 0.0);


    v1 = find_fqgrid_stack(name_v1);
    if (v1 == NULL) 
    {
        error_handler("in contr_vdiaggv(), fqgrid '%s' is not found in the stack", name_v1);
    }    
    g = find_fqgrid_stack(name_g);
    if (g == NULL) 
    {
        error_handler("in contr_vdiaggv(), fqgrid '%s' is not found in the stack", name_g);
    }      
    v2 = find_fqgrid_stack(name_v2);
    if (v2 == NULL) 
    {
        error_handler("in contr_vdiaggv(), fqgrid '%s' is not found in the stack", name_v2);
    }     

    v1gv2 = contr_fqgrid_vdiaggv(v1, is_conj, g, v2);
    
    return v1gv2;

}


double complex contr_fqgrid_vdiaggv(fqgrid_t* v1, enum CONCAT_CONJ is_conj, fqgrid_t* g, fqgrid_t* v2)
{
    // diag g(n,n)
    // v1(n)
    // v2(n)
    int n = v2->size_tot;

    double complex* gv2w = (double complex*) malloc(n * sizeof(double complex));


    double* w2 = v2->qgrid[0]->w;
    for (int i=0; i < n; i++)
    {
        double complex v2i = *(v2->data+i);
        double complex gi = *(g->data+i);
        double wi = *(w2+i);
        *(gv2w+i) =  gi*v2i*wi;
    }

    double complex v1gv2;
    cblas_zdotu_sub(n, v1->data, 1, gv2w, 1, &v1gv2);
    
    free(gv2w);

    return v1gv2;

}