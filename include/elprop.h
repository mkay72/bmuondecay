#ifndef ELPROP_H_INCLUDED
#define ELPROP_H_INCLUDED

#include <complex.h>
#include "fqgrid.h"

void contr_dxg_diagv_dxg_type1(char* name_1, char* name_v, char* name_2, char* name_target);

void contr_gxgvdxg_gfactor(char* name_1, int kappa_1, char* name_v, char* name_2, int kappa_2, char* name_target);

void append_suffix(char name[FQGRID_MAX_NAME], char* s, char name_new[FQGRID_MAX_NAME]);

void eval_dfg(double complex omega, double kappa, char* name);

void eval_dfg_amos(double complex omega, double kappa, char* name);

void eval_freg_acb(double znucl, double kappa, double complex omega, char* name);

void eval_firr_acb(double znucl, double kappa, double complex omega, char* name);

void eval_dcg(double complex omega, double znucl, double kappa, char* elprop_name);

double complex eval_wrnsk_dcg(double complex omega, double kappa, double znucl);

void axpy_dxg(double complex a, char* name_1, char* name_2, int beta_1, int beta_2, char* name_target);

void contr_gvg_dxg(char* name_a, int beta_a_1, int beta_a_2, char* name_v, char* name_b, int beta_b_1, int beta_b_2, char* name_target);

char* get_name_beta_dxg(char* name, int beta_1, int beta_2);

void new_dxg(char* name);

fqgrid_t *get_fqgrid_beta_dxg(char* name, int beta_1, int beta_2);

#endif