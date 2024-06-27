#include "qgrid.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error_handler.h"


void pretabulate_gauss_legendre(void);

extern double* gl_table[2][65];



static qgrid_t* qgrid_stack[MAX_QGRID_STACK];
static int qgrid_stack_top;


qgrid_t *qgrid_stack_push(qgrid_t* qgrid)
{
    qgrid_stack[qgrid_stack_top++] = qgrid;
    return qgrid;
}


int find_index_qgrid_stack(char *name)
{
    int i;

    for (i = 0; i < qgrid_stack_top; i++) 
    {
        if (strcmp(name, qgrid_stack[i]->name) == 0) 
        {
            return i;
        }
    }
    return -1;
}


qgrid_t* find_qgrid_stack(char *name)
{
    int i;

    for (i = 0; i < qgrid_stack_top; i++) 
    {
        if (strcmp(name, qgrid_stack[i]->name) == 0) 
        {
            return qgrid_stack[i];
        }
    }

    return NULL;
}



void print_qgrid_stack()
{
    qgrid_t* qgrid;

    if (qgrid_stack_top == 0) 
    {
        printf("print_qgrid_stack: stack is empty\n");
        return;
    }    
    
    printf("\nqgrid stack:\n");
    printf("     name    nelem     start                     end   \n");
    
    for (int i = qgrid_stack_top - 1; i >= 0; i--)
    {
        qgrid = qgrid_stack[i];

        printf("%-5d%-8s%-10d%-26.16e%-26.16e \n", i, qgrid->name, qgrid->n, *(qgrid->x), *(qgrid->x+qgrid->n-1));

    }
}


void print_qgrid(char* name)
{
    qgrid_t* qgrid = find_qgrid_stack(name);

    printf("\n# qgrid %s\n", qgrid->name);
    printf("# %+2s\t%-16s\t%-16s\n", "i", "x[i]", "w[i]");
    int size = qgrid->n;
    for (int j=0; j < size; j++)
    {
        printf("%4d\t%.16f\t%.16f\n", j, *(qgrid->x + j), *(qgrid->w + j));
    }
}

// void print_qgrid_wolfram(char* name)
// {
//     qgrid_t* qgrid = find_qgrid_stack(name);
// 
//     printf("\nqgrid %s\n", qgrid->name);
//     int size = qgrid->n;
//     printf("{");
//     for (int j=0; j < size; j++)
//     {
//         printf("%.16f, ", j, *(qgrid->x + j));
//     }
//     printf("};\n");
// }


// qgrid_t* new_qgrid(double* y, int ny, int nsub, int nqnode, char* name)
// {
//     qgrid_t* qgrid = (qgrid_t*) malloc(1 * sizeof(qgrid_t));
    
//     // yy -- array y devided by subintervals
//     int nyy = ny + (ny-1)*nsub;
//     double* yy = (double*) malloc(nyy * sizeof(double));

//     for (int i=0; i < ny-1; i++)
//     {
//         double a = *(y+i);
//         double b = *(y+i+1);
//         double* xs = uniform_split_interval(a, b, nsub);
        
//         for (int j=0; j < (nsub + 1); j++)
//         {
//             *(yy + i*(nsub+1) + j) = xs[j];
//         }
//         free(xs);
//     }
//     *(yy+nyy-1) = *(y+ny-1);


//     // total number of quadrature nodes
//     int nqnode_tot = (nyy-1)*nqnode;
//     qgrid->n = nqnode_tot;

//     qgrid->x = (double*) malloc(nqnode_tot * sizeof(double));
//     qgrid->w = (double*) malloc(nqnode_tot * sizeof(double));


//     // calculate quadrature nodes
//     for (int i=0; i < nyy-1; i++)
//     {
//         double qs[nqnode];
//         double ws[nqnode];
//         qgrid_ab(*(yy+i), *(yy+i+1), nqnode, qs, ws);
//         for (int k=0; k < nqnode; k++)
//         {
//             *(qgrid->x + i*nqnode + k) = qs[k];
//             *(qgrid->w + i*nqnode + k) = ws[k];
//         }
//     }

//     // total number of points
//     int npnt_tot = nqnode_tot + nyy;
//     qgrid->npnt = npnt_tot;
//     qgrid->pnt = malloc(npnt_tot * sizeof(double));

//     // fill array qgrid->pnt with border points
//     int k=0;
//     for (int i=0; i < npnt_tot; i += (nqnode+1))
//     {
//         *(qgrid->pnt + i) =  *(yy + k);
//         k+= 1;
//     }

//     free(yy);

//     // fill array qgrid->pnt with quadrature nodes
//     k=0;
//     for (int i=0; i < npnt_tot; i++)
//     {
//         if (i % (nqnode+1) == 0)
//         {
//             k++;
//             continue;
//         }
//         *(qgrid->pnt  + i) = *(qgrid->x + i - k);
//     }
    
//     strncpy(qgrid->name, name, QGRID_MAX_NAME);
//     qgrid->name[QGRID_MAX_NAME - 1] = '\0';   

//     return qgrid;
// }



qgrid_t* new_qgrid(double* y, int ny, int nsub, int nqnode, char* name)
{
    qgrid_t* qgrid = (qgrid_t*) malloc(1 * sizeof(qgrid_t));
    
    // yy -- array y devided by subintervals
    int nyy = ny + (ny-1)*nsub;
    double* yy = (double*) malloc(nyy * sizeof(double));

    for (int i=0; i < ny-1; i++)
    {
        double a = *(y+i);
        double b = *(y+i+1);
        double* xs = uniform_split_interval(a, b, nsub);
        
        for (int j=0; j < (nsub + 1); j++)
        {
            *(yy + i*(nsub+1) + j) = xs[j];
        }
        free(xs);
    }
    *(yy+nyy-1) = *(y+ny-1);


    // total number of quadrature nodes
    int nqnode_tot = (nyy-1)*nqnode;
    qgrid->n = nqnode_tot;

    qgrid->x = (double*) malloc(nqnode_tot * sizeof(double));
    qgrid->w = (double*) malloc(nqnode_tot * sizeof(double));


    // calculate quadrature nodes
    for (int i=0; i < nyy-1; i++)
    {
        double qs[nqnode];
        double ws[nqnode];
        qgrid_ab(*(yy+i), *(yy+i+1), nqnode, qs, ws);
        for (int k=0; k < nqnode; k++)
        {
            *(qgrid->x + i*nqnode + k) = qs[k];
            *(qgrid->w + i*nqnode + k) = ws[k];
        }
    }

    // total number of points
    int npnt_tot = nqnode_tot + nyy;
    qgrid->npnt = npnt_tot;
    qgrid->pnt = malloc(npnt_tot * sizeof(double));

    // fill array qgrid->pnt with border points
    int k=0;
    for (int i=0; i < npnt_tot; i += (nqnode+1))
    {
        *(qgrid->pnt + i) =  *(yy + k);
        k+= 1;
    }

    free(yy);

    // fill array qgrid->pnt with quadrature nodes
    k=0;
    for (int i=0; i < npnt_tot; i++)
    {
        if (i % (nqnode+1) == 0)
        {
            k++;
            continue;
        }
        *(qgrid->pnt  + i) = *(qgrid->x + i - k);
    }
    
    strncpy(qgrid->name, name, QGRID_MAX_NAME);
    qgrid->name[QGRID_MAX_NAME - 1] = '\0';   

    return qgrid;
}


/*
 * returns n quadrature nodes qs and weights ws 
 * between points a and b
 * arrays qs and ws must be preallocated
 */

// void qgrid_ab(double a, double b, int n, double* qs, double* ws)
// {   
//     gsl_integration_fixed_workspace *qw;
//     qw = gsl_integration_fixed_alloc(gsl_integration_fixed_legendre, n, a, b, 1.0, 1.0);
    
//     double* nodes = gsl_integration_fixed_nodes(qw);
//     double* weights = gsl_integration_fixed_weights(qw);
        
//     for (int i=0; i < n; i++)
//     {
//         *(qs+i) = *(nodes+i);
//         *(ws+i) = *(weights+i);
//     }
    
//     gsl_integration_fixed_free(qw);
// }


void qgrid_ab(double a, double b, int n, double* qs, double* ws)
{   
    if (n > 64)
    {
        error_handler("in qgrid_ab(), number of nodes n = %d > 64.\nThe maximum number of the Gauss-Legendre nodes in an interval is 64.", n);
    }
    else if (n < 2)
    {
        error_handler("in qgrid_ab(), number of nodes n = %d < 2.\nThe minimun number of the Gauss-Legendre nodes in an interval is 2.", n);
    }    
    for (int i=0; i < n; i++)
    {
        *(qs+i) = 0.5 * (b-a) * gl_table[0][n][i] + (0.5) * (b+a);
        *(ws+i) = 0.5 * (b-a) * gl_table[1][n][i];
    }
}




double* uniform_split_interval(double a, double b, int nsub) 
{
    if (nsub >= 0) 
    {
        double* sub = (double*) malloc((nsub+1) * sizeof(double));

        double sub_len = (b - a) / (nsub+1);

        for (int i = 0; i < (nsub+1); i++) 
        {
            sub[i] = a + i*sub_len;
        }
    return sub;
    }
}


void delete_qgrid(qgrid_t* qgrid)
{
    free(qgrid->pnt);
    free(qgrid->x);
    free(qgrid->w);
    // free(qgrid->name);
    free(qgrid);

}
