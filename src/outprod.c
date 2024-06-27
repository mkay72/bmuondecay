#include "outprod.h"

// void outpropd(char* name1, char* name2, char* target)
// {
//     // https://stackoverflow.com/questions/19845071/outer-product-using-cblas   
//     fqgrid_t* v1;
//     fqgrid_t* v2;
    
//     v1 = find_fqgrid_stack(name1);
//     if (v1 == NULL) 
//     {
//         error_handler("in concat(), fqgrid '%s' is not found in the stack", name1);
//     }    
//     v2 = find_fqgrid_stack(name2);
//     if (v2 == NULL) 
//     {
//         error_handler("in concat(), fqgrid '%s' is not found in the stack", name2);
//     }

//     g1vg2 = concat_gvg_fqgrid(g1, is_conj, v, g2);

// cblas_zgeru(CblasRowMajor, /* you’re using row-major storage */
//            dx1,           /* the matrix X has dx1 rows ...  */
//            dx2,           /*  ... and dx2 columns.          */
//            1.0,           /* scale factor to apply to x1x2' */
//            x1,
//            1,             /* stride between elements of x1. */
//            x2,
//            1,             /* stride between elements of x2. */
//            X,
//            dx2);          /* leading dimension of matrix X. */

// }