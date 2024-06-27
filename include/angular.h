#ifndef ANGULAR_H_INCLUDED
#define ANGULAR_H_INCLUDED


extern void hfd_cgc_(double* j1, double* m1, double* j2, double* m2, double* j3, double* m3, double* res);

double cgc(double j1, double m1, double j2, double m2,  double j3, double m3);

double cgc_mzero(int l1, int l2, int l3);

double pp(double j);

double jkappa(int kappa);

int akappaj(double j);

int lkappa(int kappa);

int lbarkappa(int kappa);

int is_even(int n);

int is_odd(int n);

double ulbeta(int l, int kappa_a, int kappa_b, int lcapital, int beta);

double slbeta(int l, int kappa_a, int kappa_b, int lcapital, int beta);

int is_triangle(double j_a, double j_b, double j_c);

#endif /* ANGULAR_H_INCLUDED */