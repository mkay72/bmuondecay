#ifndef BMUONONEBODYDECAY_H_INCLUDED
#define BMUONONEBODYDECAY_H_INCLUDED

#include <string.h>

double eval_radint_type_1(char* name_c, char* name_d, char* name_a, char* name_b);

double eval_radint_type_2(char* name_c, char* name_d, char* name_a, char* name_b);

double eval_radint_type_3(char* name_c, char* name_d, char* name_a, char* name_b);

double eval_radint_type_4(char* name_c, char* name_d, char* name_a, char* name_b);

double eval_radint(char* name_c, int beta_c, char* name_d, int beta_d, char* name_a, int beta_a, char* name_b, int beta_b);

static double partial_transrate_mu_decay(int znucl, int n_muon, int kappa_muon, double e_e, int akappa_e);

int bmuon_decay_onebody(void);


#endif