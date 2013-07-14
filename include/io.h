#ifndef LUMA_IO_H
#define LUMA_IO_H

#include <stdio.h>


// Logging --------------------------------------------------------------------
// ----------------------------------------------------------------------------
#define io_log(format, args...) _log(io, format, ##args)
#define io_info(format, args...) _info(io, format, ##args) 
#define io_warn(format, args...) _warn(io, format, ##args) 
#define io_error(format, args...) _error(io, format, ##args)


// Structures -----------------------------------------------------------------
// ----------------------------------------------------------------------------



// API ------------------------------------------------------------------------
// ----------------------------------------------------------------------------
char *io_load_resource(const char* filename, unsigned int *length);


#endif

