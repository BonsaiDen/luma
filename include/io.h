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
struct IOResource;
typedef struct IOResource {
    char *filename;
    unsigned int size;
    void *handle;
    void(*release)(struct IOResource *res);

} IOResource;


// API ------------------------------------------------------------------------
// ----------------------------------------------------------------------------
IOResource *io_load_text(const char* filename);

bool io_init();
bool io_shutdown();

#endif

