#ifndef FQGRID_STACK_H_INCLUDED
#define FQGRID_STACK_H_INCLUDED

#include "fqgrid.h"

#define FQGRID_MAX_STACK_DEPTH 4*2048

typedef int fqgrid_stack_pos_t;

fqgrid_stack_pos_t get_stack_pos();

void restore_stack_pos(fqgrid_stack_pos_t pos);

int find_index_fqgrid_stack(char *name);

fqgrid_t* find_fqgrid_stack(char *name);

fqgrid_t *replace_stack_fqgrid(char *name, fqgrid_t *fqgrid);

fqgrid_t* push_fqgrid_stack(fqgrid_t *fqgrid);

void print_fqgrid_stack();

void delete_fqgrid_stack(char *name);

// void purge_fqgrid_stack();

// fqgrid_stacktop = 0;


// diagram_t *diagram_stack_push(diagram_t *dg)
// {
//     if (dg_stack_top == CC_MAX_STACK_DEPTH) {
//         errquit("diagram_stack_push: stack overflow "
//                 "(please, increase the CC_MAX_STACK_DEPTH parameter, currently == %d)",
//                 CC_MAX_STACK_DEPTH);
//     }

//     dg_stack[dg_stack_top++] = dg;
//     return dg;
// }

#endif /*FQGRID_STACK_H_INCLUDED*/