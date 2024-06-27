#ifndef MULTPTWS_H_INCLUDED
#define MULTPTWS_H_INCLUDED

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#else
#include <cblas.h>
#endif
#include <complex.h>


void multptws(char* name_fqgrid_1, char* name_fqgrid_2, char* target);

fqgrid_t* multptws_fqg(fqgrid_t* fqg1, fqgrid_t* fqg2);

void multptws_fqgrid_type_vv_gg(fqgrid_t* fqg1, fqgrid_t* fqg2, fqgrid_t* fqg1fqg2);

void multptws_fqgrid_type_vg(fqgrid_t* fqg1, fqgrid_t* fqg2, fqgrid_t* fqg1fqg2);

void multptws_fqgrid_type_gv(fqgrid_t* fqg1, fqgrid_t* fqg2, fqgrid_t* fqg1fqg2);

void multptws_fqg_is_valid(fqgrid_t* fqg1, fqgrid_t* fqg2);

typedef enum MULTPTWS_FQG_OPERATION_TYPE 
{
    VV, // f(x) * g(x)
    VG, // f(x) * a(x,y)
    GV, // a(x,y) * f(y)
    GG  // a(x,y) * b(x,y)
} MULTPTWS_FQG_OPERATION_TYPE;

enum MULTPTWS_FQG_OPERATION_TYPE multptws_fqg_select_operation_type(fqgrid_t* fqg1, fqgrid_t* fqg2);

fqgrid_t* multptws_fqg_create_template(fqgrid_t* fqgrid_g1, fqgrid_t* fqgrid_g2, enum MULTPTWS_FQG_OPERATION_TYPE type);

void scale_i(double complex a, char* name_fqgrid_1);

void scale_i_fqg(double complex a, fqgrid_t* fqg);


double vecnorm_L1(char* name_fqg);

double vecnorm_L1_fqg(fqgrid_t* fqg);

#endif
