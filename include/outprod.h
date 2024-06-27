#ifndef OUTPROD_H_INCLUDED
#define OUTPROD_H_INCLUDED 

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#else
#include <cblas.h>
#endif

#include <complex.h>

void outpropd(char* name1, char* name2, char* target);




#endif
