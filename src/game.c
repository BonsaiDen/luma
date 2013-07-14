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
        }

        free(filename);

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
const char *internal_get_working_dir() {
    
    char buf[PATH_MAX];
    const char *cwd;
    if ((cwd = getcwd(buf, sizeof(buf))) == NULL) {
        return NULL;

    } else {
        return cwd;
    }

}

bool internal_find_module(int argc, char **argv, char **module) {

    // Retrieve working directory and the passed path
    const char *cwd = internal_get_working_dir();
    char *target = argc > 1 ? argv[1] : "";
    char *path = (char*)calloc(strlen(target) + strlen(cwd) + 2, sizeof(char));
    char *filename;

    strcat(path, cwd);
    if (strlen(target) > 0) {
        strcat(path, "/");
        strcat(path, target);
    }

    char *pos = strrchr(path, '/');
    unsigned int offset = pos ? pos - path : 0;

    // Detect whether the target exists and is a file or directory
    struct stat s;
    if (stat(path, &s) == 0) {

        // If it's a directoy, we set the default file to "main.lua"
        if (S_ISDIR(s.st_mode)) {
            filename = (char*)calloc(8 + 1, sizeof(char));
            strcat(filename, "main.lua");
            if (offset == strlen(path) - 1) {
                path[offset] = '\0';
            }
            printf("%s, %d, %d\n", path, offset, strlen(path));
            
        // If it's a file, we change the path to the parent and use it as the filename
        } else if (S_ISREG(s.st_mode)) {
            filename = (char*)calloc(strlen(path) - offset, sizeof(char));
            strcpy(filename, path + offset + 1);
            path[offset] = '\0';
        }

    } else {
        game_error("invalid target '%s'", path);
        free(path);
        return false;
    }

    // Build the full module path
    *module = (char*)calloc(strlen(path) + 2 + strlen(filename), sizeof(char));
    strcat(*module, path);
    strcat(*module, "/");
    strcat(*module, filename);

    // Verify that the target is indeed a file
    if (stat(*module, &s) == 0 && S_ISREG(s.st_mode)) {
        game_info("init module '%s'", *module);
        chdir(path);
        free(path);
        free(filename);
        return true;

    } else {
        game_error("invalid module '%s'", *module);
        free(path);
        free(filename);
        free(module);
        return false;
    }

}

