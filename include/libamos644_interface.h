#ifndef LIBAMOS644_INTERFACE_H_INCLUDED
#define LIBAMOS644_INTERFACE_H_INCLUDED

// #include "constants.h"
#include "fqgrid.h"
#include <complex.h>
// #include "qgrid.h"

// double complex conhyp_(double complex *a, double complex *b, double complex *z, int* lnchf,int *ip);

// complex cpsi_(complex *z);

void zbesh_(double *zr, double *zi, double *fnu, int *kode, int *m, int *n, double *cyr, double *cyi, int *nz, int *nerr);

void zbesj_(double *zr, double *zi, double *fnu, int *kode, int *n, double *cyr, double *cyi, int *nz, int *nerr); 

void zbesy_(double *zr, double *zi, double *fnu, int *kode, int *n, double *cyr, double *cyi, int *nz, double *cwrkr, double *cwrki, int *nerr);


void zspbesj_map(double fnu, double complex *zs, int zn, double complex *ys);

void zspbesy_map(double fnu, double complex *zs, int zn, double complex *ys);

// void dspbesj_map(double fnu, double *xs, int xn, double *ys);
// 
void zspbesh1_map(double fnu, double complex *zs, int zn, double complex *ys);
// 
// void dspbesh1_map(double fnu, double *xs, int xn, double *ys);
// 
// void zspbesx_map(char kind, double complex omega, double l, zfqgrid_t* zspbesxs, int dim);

#endif  /* LIBAMOS644_INTERFACE_H_INCLUDED */
