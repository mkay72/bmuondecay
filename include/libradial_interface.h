#ifndef LIBRADIAL_INTERFACE_H_INCLUDED
#define LIBRADIAL_INTERFACE_H_INCLUDED


// void vpot_fermi_(double* znucl, double* rms_fm, double* mass, int* nr, double* r, double* vr);


extern void radial_dbound_(double* z, double* rms_fm, int* knu, double* m, int* n, int *kp, int* nr, double* r_in, double* g_out, double* f_out, double* e_out);

extern void radial_dfree_(double* z, double* rms_fm, int* knu, double* m, double* e, int *kp, int* nr, double* r_in, double* g_out, double* f_out, double* phase);

double eval_dbound_radial(int z, double rms_fm, int knu, double mass, int n, int kappa, char* name);

double eval_dfree_radial(int z, double rms_fm, int knu, double mass, double e, int kappa, char* name);



#endif