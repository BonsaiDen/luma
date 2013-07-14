#include "../include/game.h"
#include "../include/vm.h"


#include <sys/stat.h>
#include <unistd.h>

// Variables ------------------------------------------------------------------
// ----------------------------------------------------------------------------


// Structures -----------------------------------------------------------------
// ----------------------------------------------------------------------------


// API ------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool internal_find_module(int argc, char **argv, char **module);

bool game_init(int argc, char **argv) {

    bool success = false;
    if (vm_init()) {
        
        // TODO setup graphics, input, sound etc.
        char *filename;
        if (internal_find_module(argc, argv, &filename)) {

            if (vm_require(filename)) {
                game_info("init success");
                success = true;
            }

            free(filename);

        }

    }

    return success;

}

bool game_update() {
    return true;
}

bool game_shutdown() {

    if (vm_shutdown()) {
        game_info("shutdown success");
        return true;

    } else {
        game_info("shutdown failed");
        return false;
    }

}

void game_api() {
    
}


// Internal -------------------------------------------------------------------
// ----------------------------------------------------------------------------
int internal_get_type(const char *path) {
    
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

const char *internal_get_working_dir() {
    
    char buf[PATH_MAX];
    const char *cwd;
    if ((cwd = getcwd(buf, sizeof(buf))) == NULL) {
        return NULL;

    } else {
        return cwd;
    }

}

char *internal_get_path(int argc, char **argv) {

    const char *cwd = internal_get_working_dir();
    const char *target = argc > 1 ? argv[1] : "";
    char *path = (char*)calloc(strlen(target) + strlen(cwd) + 2, sizeof(char));

    strcat(path, cwd);
    if (strlen(target) > 0) {
        strcat(path, "/");
        strcat(path, target);
    }

    return path;

}

char *internal_split_path_and_filename(char **path, const int type) {

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

bool internal_find_module(int argc, char **argv, char **module) {

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
            success = true;
            game_error("invalid module '%s'", *module);
        }

    } else {
        game_error("invalid target '%s'", path);
    }

    free(path);
    return success;

}

