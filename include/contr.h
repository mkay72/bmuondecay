#ifndef CONTR_H_INCLUDED
#define CONTR_H_INCLUDED

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#else
#include <cblas.h>
#endif
#include <complex.h>

enum CONCAT_CONJ 
{
    NO_CONJ,
    CONJ
};


void contr_gvg(char* name_g1, enum CONCAT_CONJ is_conj, char* name_v, char* name_g2, char* target);

fqgrid_t* contr_gvg_fqg(fqgrid_t* fqgrid_g1, enum CONCAT_CONJ is_conj, fqgrid_t* fqgrid_v, fqgrid_t* fqgrid_g2);

fqgrid_t* contr_fqgrid_template(fqgrid_t* fqgrid_g1, fqgrid_t* fqgrid_g2);

double complex contr_vgv(char* name_v1, enum CONCAT_CONJ is_conj, char* name_g, char* name_v2);

double complex contr_fqgrid_vgv(fqgrid_t* v1, enum CONCAT_CONJ is_conj, fqgrid_t* g, fqgrid_t* v2);

void contr_gvg_fqg_is_valid(fqgrid_t* fqg1, fqgrid_t* fqgv, fqgrid_t* fqg2);


double complex contr_vdiaggv(char* name_v1, enum CONCAT_CONJ is_conj, char* name_g, char* name_v2);

double complex contr_fqgrid_vdiaggv(fqgrid_t* v1, enum CONCAT_CONJ is_conj, fqgrid_t* g, fqgrid_t* v2);
// fqgrid_t* multptws_fqgrid_template(fqgrid_t* fqgrid_g1, fqgrid_t* fqgrid_g2, enum MULTPTWS_FQGRID_TEMPLATE_TYPE type);

#endif
