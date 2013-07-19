#include "../include/game.h"
#include "../include/io.h"


// Variables ------------------------------------------------------------------
// ----------------------------------------------------------------------------
HashMap *resource_map;


// API ------------------------------------------------------------------------
// ----------------------------------------------------------------------------
IOResource *internal_get_resource(const char *filename, bool(*loader)(IOResource *res));
void internal_release_resources(const char *key, void *value);
bool internal_text_load(IOResource *res);

bool io_init() {

    resource_map = hashMap(0);
    if (resource_map != NULL) {
        io_info("init");
        return true;
        
    } else {
        io_error("failed");
        return false;
    }

}

bool io_shutdown() {
    resource_map->each(resource_map, &internal_release_resources);
    resource_map->destroy(&resource_map);
    io_info("shutdown");
    return true;
}


// Resource Loaders -----------------------------------------------------------
// ----------------------------------------------------------------------------
IOResource *io_load_text(const char* filename) {
    io_info("[res] text '%s' required", filename);
    return internal_get_resource(filename, &internal_text_load);
}


// Internal -------------------------------------------------------------------
// ----------------------------------------------------------------------------
void internal_release_resources(const char *key, void *value) {
    IOResource *res = (IOResource*)value;
    io_info("[res] '%s' released", res->filename);
    res->release(res);
    res->size = 0;
    res->handle = NULL;
    free(res->filename);
}

IOResource *internal_get_resource(const char *filename, bool(*loader)(IOResource *res)) {

    IOResource *res;
    if (resource_map->hasKey(resource_map, filename)) {
        res = resource_map->get(resource_map, filename);
        io_info("[res] '%s' loaded from cache", filename);
        
    } else {

        res = (IOResource*)malloc(sizeof(IOResource));
        res->filename = calloc(strlen(filename) + 1, sizeof(char));
        strcpy(res->filename, filename);

        if (loader(res)) {
            io_info("[res] '%s' loaded", filename);
            resource_map->set(resource_map, filename, res);
            
        } else {
            free(res);
            res = NULL;
        }

    }

    return res;

}

void internal_text_release(IOResource *res) {
    free(res->handle);
}

bool internal_text_load(IOResource *res) {
    
    FILE *f = fopen(res->filename, "r");
    if (f == NULL) {
        io_error("[res] file '%s' not found", res->filename);
        return false;

    } else {
            
        fseek(f, 0, SEEK_END);
        res->size = ftell(f);
        fseek(f, 0, SEEK_SET);
        
        res->handle = (char*)calloc(res->size, sizeof(char));

        if (fread(res->handle, sizeof(char), res->size, f) != res->size) {
            fclose(f);
            io_error("[res] file '%s' not loaded", res->filename);
            return false;

        } else {
            res->release = &internal_text_release;
            fclose(f);
            io_info("[res] file '%s' loaded", res->filename);
            return true;
        }

    }

}

