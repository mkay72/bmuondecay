#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "fqgrid.h"
#include "fqgrid_stack.h"

#include "sphbes.h"
#include "error_handler.h"
#include "axpy.h"
#include "contr.h"
#include "constants.h"
#include "multptws.h"

#include "angular.h"
#include "units.h"


#include "bmuon_decay_onebody.h"

#include "name.h"
#include "libhfd_interface.h"
#include "parser.h"
#include "parse_input.h"
#include "options.h"


void parse_main_args(int argc, char** argv, char input_name[256])
{
    if (argc > 2)
    {
        exit(0);
    }
    
    strncpy(input_name, argv[1], 256);
}



int main(int argc, char** argv)
{
    char input_name[256];
    
    parse_main_args(argc, argv, input_name);
    
    

    options_t* opts = new_options();
    options = opts;
    parse_input(input_name, options);

    print_options(options);

    bmuon_decay_onebody();
  
    delete_options(options);
    return 1;
}
