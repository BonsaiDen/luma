#include "../include/game.h"
#include "../include/io.h"
#include "../include/vm.h"
#include "../include/graphics.h"

#include <sys/stat.h>
#include <unistd.h>

// API ------------------------------------------------------------------------
// ----------------------------------------------------------------------------
static bool internal_find_module(int argc, char **argv, char **module);

bool game_init(int argc, char **argv) {

    bool success = false;
    if (io_init() && vm_init()) {
        
        char *filename;
        if (internal_find_module(argc, argv, &filename)) {

            if (vm_require(filename)) {
                if (graphics_init(320, 240, "luma")) {
                    game_info("init success");
                    success = true;
                }
            }

            free(filename);

        }

    }

    return success;

}

void internal_update(const double time) {
    //game_info("time = %.3f", time);
    //vm_update(time);
    //audio_update(time);
    //audio_update(time);
}

void internal_key_callback(GLFWwindow* window, int key, 
                           int scancode, int action, int mods) {
    
}

bool game_update() {
    return graphics_update(&internal_update, &internal_key_callback);
}

bool game_shutdown() {

    if (vm_shutdown() 
        && io_shutdown() 
        && graphics_shutdown()) {

        game_info("shutdown success");
        return true;

    } else {
        game_info("shutdown failed");
        return false;
    }

}


// Internal -------------------------------------------------------------------
// ----------------------------------------------------------------------------
static int internal_get_type(const char *path) {
    
    struct stat s;
    if (stat(path, &s) == 0) {
        if (S_ISREG(s.st_mode)) {
            return 1;

        } else if (S_ISDIR(s.st_mode)) {
            return 2;
        }
    }

    return 0;

}

char *internal_get_working_dir() {
    
    char *cwd;
    if ((cwd = getcwd(NULL, PATH_MAX)) == NULL) {
        return NULL;

    } else {
        return cwd;
    }

}

static char *internal_get_path(int argc, char **argv) {

    char *cwd = internal_get_working_dir();
    const char *target = argc > 1 ? argv[1] : "";
    char *path = (char*)calloc(strlen(target) + strlen(cwd) + 2, sizeof(char));

    strcat(path, cwd);
    if (strlen(target) > 0) {
        strcat(path, "/");
        strcat(path, target);
    }

    free(cwd);
    return path;

}

static char *internal_split_path_and_filename(char **path, const int type) {

    const char *pos = strrchr(*path, '/');
    const unsigned int offset = pos ? pos - *path : 0;
    char *filename;

    // File
    if (type == 1) {
        filename = (char*)calloc(strlen(*path) - offset, sizeof(char));
        strcpy(filename, *path + offset + 1);
        *path[offset] = '\0';

    // Directory
    } else if (type == 2) {
        filename = (char*)calloc(8 + 1, sizeof(char));
        strcat(filename, "main.lua");
        if (offset == strlen(*path) - 1) {
            *path[offset] = '\0';
        }
    }

    return filename;

}

static bool internal_find_module(int argc, char **argv, char **module) {

    char *path = internal_get_path(argc, argv);
    char *filename;
    const int type = internal_get_type(path);
    bool success = false;

    if (type != 0) {

        filename = internal_split_path_and_filename(&path, type);

        *module = (char*)calloc(strlen(path) + 2 + strlen(filename), sizeof(char));
        strcat(*module, path);
        strcat(*module, "/");
        strcat(*module, filename);
        free(filename);

        if (internal_get_type(*module) == 1) {
            chdir(path);
            success = true;
            game_info("init module '%s'", *module);
            
        } else {
            free(*module);
            game_error("invalid module '%s'", *module);
        }

    } else {
        game_error("invalid target '%s'", path);
    }

    free(path);
    return success;

}

