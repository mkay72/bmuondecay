#include "fqgrid_stack.h"

#include <stdio.h>
#include <string.h>

static fqgrid_t *fqgrid_stack[FQGRID_MAX_STACK_DEPTH];
static int fqgrid_stack_top = 0;

fqgrid_t* push_fqgrid_stack(fqgrid_t *fqgrid)
{

    fqgrid_stack[fqgrid_stack_top++] = fqgrid;
    return fqgrid;
}



int find_index_fqgrid_stack(char *name)
{
    int i;

    for (i = 0; i < fqgrid_stack_top; i++) 
    {
        if (strcmp(name, fqgrid_stack[i]->name) == 0) 
        {
            return i;
        }
    }
    return -1;
}


fqgrid_t* find_fqgrid_stack(char *name)
{
    int i;

    for (i = 0; i < fqgrid_stack_top; i++) 
    {
        if (strcmp(name, fqgrid_stack[i]->name) == 0) 
        {
            return fqgrid_stack[i];
        }
    }

    return NULL;
}


void print_fqgrid_stack()
{
    fqgrid_t* fqgrid;

    if (fqgrid_stack_top == 0) 
    {
        printf("print_fqgrid_stack: stack is empty\n");
        return;
    }    
    
    printf("\nfqgrid stack:\n");
    printf("    id   name                            nelem     size [Mb]   \n");
    
    for (int i = fqgrid_stack_top - 1; i >= 0; i--)
    {
        fqgrid = fqgrid_stack[i];

        printf("%-4d%-5d%-32s%-10d%-10.3f \n", i, fqgrid->id, fqgrid->name, fqgrid->size_tot, (fqgrid->size_tot)*sizeof(double complex)/1024.0/1024.0);

    }
}


fqgrid_t *replace_stack_fqgrid(char *name, fqgrid_t *fqgrid)
{
    fqgrid_t *fqgrid_old;
    int i;

    i = find_index_fqgrid_stack(name);
    if (i == -1) 
    {
        return NULL;
    }

    fqgrid_old = fqgrid_stack[i];
    fqgrid_stack[i] = fqgrid;

    delete_fqgrid(fqgrid_old);

    return fqgrid;
}



void delete_fqgrid_stack(char *name)
{
    int i;
    fqgrid_t *fqgrid = NULL;

    for (i = 0; i < fqgrid_stack_top; i++) {
        if (strcmp(name, fqgrid_stack[i]->name) == 0) 
        {
            fqgrid = fqgrid_stack[i];

            for (int j = i; j < fqgrid_stack_top - 1; j++) 
            {
                fqgrid_stack[j] = fqgrid_stack[j + 1];
            }
            fqgrid_stack_top--;

            break;
        }
    }

    if (fqgrid != NULL) 
    {
        delete_fqgrid(fqgrid);
    }
}



fqgrid_stack_pos_t get_stack_pos()
{
    return fqgrid_stack_top;
}


void restore_stack_pos(fqgrid_stack_pos_t pos)
{
    int i;

    for (i = pos; i < fqgrid_stack_top; i++) {
        delete_fqgrid(fqgrid_stack[i]);
    }
    fqgrid_stack_top = pos;
}