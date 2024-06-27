#ifndef QGRID_H_INCLUDED
#define QGRID_H_INCLUDED

// #include <gsl/gsl_integration.h>

#define MAX_QGRID_STACK 5
#define QGRID_MAX_NAME 8

typedef struct
{
    double *x;
    double *w;
    int n;
    double *pnt;
    int npnt;

    char name[QGRID_MAX_NAME];

} qgrid_t;


qgrid_t *qgrid_stack_push(qgrid_t* qgrid);

int find_index_qgrid_stack(char *name);

qgrid_t* find_qgrid_stack(char *name);

void print_qgrid_stack();

void print_qgrid_wolfram(char* name);

/*
 * @brief 
 * 
 *
 * @param y 
 * @param ny 
 * @param nsub 
 * @param nqnodes 
 * @return 
 */
qgrid_t* new_qgrid(double* y, int ny, int nsub, int nqnodes, char* name);

qgrid_t* new_qgrid_table(double* y, int ny, int nsub, int nqnodes, char* name);

void delete_qgrid(qgrid_t* qgrid);

void print_qgrid(char* name);

void uniform_interval(double a, double b, int n, double* x);

void qgrid_ab(double a, double b, int n, double* qs, double* ws);

void qgrid_ab_table(double a, double b, int n, double* qs, double* ws);

double* uniform_split_interval(double a, double b, int nsubintervals);

#endif /*QGRID_H_INCLUDED*/