#include "../include/game.h"
#include "../include/io.h"


// Variables ------------------------------------------------------------------
// ----------------------------------------------------------------------------


// Structures -----------------------------------------------------------------
// ----------------------------------------------------------------------------


// API ------------------------------------------------------------------------
// ----------------------------------------------------------------------------
char *internal_load_raw(const char* filename, unsigned int *length);
char *internal_load_zip(const char* filename, unsigned int *length);

bool io_init() {
    io_info("init");
    return true;
}

bool io_update() {
    io_info("update");
    return true;
}

bool io_shutdown() {
    io_info("shutdown");
    return true;
}


char *io_load_resource(const char* filename, unsigned int *length) {
    io_info("resource '%s' requested", filename);
    return internal_load_raw(filename, length);
}


// Internal -------------------------------------------------------------------
// ----------------------------------------------------------------------------
char *internal_load_raw(const char* filename, unsigned int *length) {
    
    FILE *f = fopen(filename, "r");
    char *buf;

    if (f == NULL) {
        io_error("file '%s' not found", filename);
        return NULL;

    } else {
            
        fseek(f, 0, SEEK_END);
        *length = ftell(f);
        fseek(f, 0, SEEK_SET);
        
        buf = (char*)calloc(*length + 1, sizeof(char));
        if (fread(buf, sizeof(char), *length, f) != *length) {
            io_error("file '%s' not loaded", filename);
            return NULL;

        } else {
            fclose(f);
            io_info("file '%s' loaded", filename);
            return buf;
        }

    }

}

char *internal_load_zip(const char* filename, unsigned int *length) {
    return NULL;
}

