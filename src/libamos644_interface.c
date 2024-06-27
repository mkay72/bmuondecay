#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <math.h>

#include "libamos644_interface.h"
#include "constants.h"

// #include "fgrid.h"
// #include "qgrid.h"


void zspbesj_map(double fnu, double complex *zs, int zn, double complex *ys)
{
  fnu += 0.5;
  int kode = 1;
  int n = 1;
  int nz;
  int nerr;
  
  double complex z;
  double zr, zi;
   
  double cyr = 0.0;
  double cyi = 0.0;
  
  for (int i=0; i < zn; i++)
  {
    z = *(zs+i);
    zr = creal(z);
    zi = cimag(z);
    
    zbesj_(&zr, &zi, &fnu, &kode, &n, &cyr, &cyi, &nz, &nerr);
    *(ys+i) = SQRT_PI_OVER_2 * CMPLX(cyr, cyi) / csqrt(z);
    // printf("nerr = %d\n", nerr);
    // printf("%e %e\n", *(ys+i));
  }

  // printf("ys from zspbesj_map\n");
  // for (int i=0; i < zn; i++)
  // {
  //   printf("%e %e\n", *(ys+i));
  // }
  // printf("\n");

} 


void zspbesy_map(double fnu, double complex *zs, int zn, double complex *ys)
{
  fnu += 0.5;
  int kode = 1;
  int n = 1;
  int nz;
  int nerr;
  
  double complex z;
  double zr, zi;
   
  double cyr = 0.0;
  double cyi = 0.0;

  double cwrkr[zn];
  double cwrki[zn];
  
  for (int i=0; i < zn; i++)
  {
    z = *(zs+i);
    zr = creal(z);
    zi = cimag(z);
    
    zbesy_(&zr, &zi, &fnu, &kode, &n, &cyr, &cyi, &nz, cwrkr, cwrki, &nerr);
    *(ys+i) = SQRT_PI_OVER_2 * CMPLX(cyr, cyi) / csqrt(z);
  }
} 
    
    

// void dspbesj_map(double fnu, double *xs, int xn, double *ys)
// {
//   fnu += 0.5;
//   int kode = 1;
//   int n = 1;
//   int nz = 0;
//   int nerr = 0;
//   double zero = 0.0;
//   
//   double *x0;
//   double *cyr = malloc(2*n * sizeof(double));
//   double *cyi = cyr+n;
//   
//   for (int i=0; i < xn; i++)
//   {
//     x0 = xs+i;
//     zbesj_(x0, &zero, &fnu, &kode, &n, cyr, cyi, &nz, &nerr);
//     *(ys+i) = *cyr / sqrt(*x0) * SQRT_PI_OVER_2;
//   }
//   
//   free(cyr);
//   free(cyi);
// }
// 
void zspbesh1_map(double fnu, double complex *zs, int zn, double complex *ys)
{
  fnu += 0.5;
  int kode = 1;
  int m = 1;
  int n = 1;
  int nz;
  int nerr;
  
  double complex z;
  double zs_r;
  double zs_i;
  
  double *cyr = malloc(n * sizeof(double));
  double *cyi = malloc(n * sizeof(double));
  
  for (int i=0; i < zn; i++)
  {
    z = *(zs+i);
    zs_r = creal(*(zs+i));
    zs_i = cimag(*(zs+i));
    
    zbesh_(&zs_r, &zs_i, &fnu, &kode, &m, &n, cyr, cyi, &nz, &nerr);
    *(ys+i) = SQRT_PI_OVER_2 * CMPLX(*cyr, *cyi) / csqrt(z);
//     printf("%f %f\n", *cyr, *cyi);
  }
  
  free(cyr);
  free(cyi);
}


// 
// 
// void dspbesh1_map(double fnu, double *xs, int xn, double *ys)
// {
//   fnu += 0.5;
//   int kode = 1;
//   int n = 1;
//   int m = 1;
//   int nz = 0;
//   int nerr = 0;
//   double zero = 0.0;
//   
//   double *x0;
//   double *cyr = malloc(n * sizeof(double));
//   double *cyi = malloc(n * sizeof(double));
//   
//   for (int i=0; i < xn; i++)
//   {
//     x0 = xs+i;
//     zbesh_(x0, &zero, &fnu, &kode, &m, &n, cyr, cyi, &nz, &nerr);
//     *(ys+i) = *cyr / sqrt(*x0) * SQRTPIOVER2;
//   }
//   
//   free(cyr);
//   free(cyi);
// } 
// 
// 

// void zspbesx_map(char kind, double complex omega, double l, double complex* sphbes)
// {    
//     if ((kind != 'j') && (kind != 'h') && (kind != 'y'))
//     {
//         printf("Incorrect kind of Bessel function j, h1 or y.\n");
//         exit(-1);
//     }
    
//     int size = zspbesxs->grid->sizes[dim];
//     double complex *zs = malloc(size * sizeof(double complex));
    
//     double *xs = zspbesxs->grid->xs[dim];
//     for (int i=0; i < size; i++)
//     {
//         *(zs+i) = *(xs + i) * omega;
//     }
    
//     if (kind == 'j')
//     {
//         zspbesj_map(l, zs, size, zspbesxs->fs[dim]);
//     }
//     else if (kind == 'h')
//     {
//         zspbesh1_map(l, zs, size, zspbesxs->fs[dim]);
//     }
//     else if (kind == 'y')
//     {
//         zspbesy_map(l, zs, size, zspbesxs->fs[dim]);      
//     }
    
//     free(zs);
// }
