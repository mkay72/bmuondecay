#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "error_handler.h"


void error_handler(const char* message, ...) 
{
    va_list args;
    va_start(args, message);

    // Define the error buffer inside the function
    int buffer_size = vsnprintf(NULL, 0, message, args) + 1;
    char error_buffer[buffer_size];
    memset(error_buffer, 0, buffer_size);

    // Format the error message
    va_start(args, message);  // Restart argument list processing
    vsnprintf(error_buffer, buffer_size, message, args);

    va_end(args);

    // Print error message to stderr
    fprintf(stderr, "\nError: %s\n", error_buffer);

    exit(EXIT_FAILURE);
}
