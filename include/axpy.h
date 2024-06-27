#ifndef AXPY_H_INCLUDED
#define AXPY_H_INCLUDED



fqgrid_t* axpy_fqgrid(double complex a, fqgrid_t* fqgrid_x, fqgrid_t* fqgrid_y);

void axpy(double complex a, char* name1, char* name2, char* target);

#endif