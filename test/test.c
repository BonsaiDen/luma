#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../deps/include/glfw3.h"
#include "../deps/include/png.h"
#include "../deps/include/portaudio.h"
#include "../deps/include/zlib.h"
#include "../deps/include/lua.h"
#include "../deps/include/lualib.h"
#include "../deps/include/lauxlib.h"

int main(int argc, char **argv) {

    int ret = 0;

    printf(" # Running Tests...\n");

    // Test zlib --------------------------------------------------------------
    // ------------------------------------------------------------------------
    z_stream zstrm;
    zstrm.zalloc = Z_NULL;
    zstrm.zfree = Z_NULL;
    zstrm.opaque = Z_NULL;

    if (deflateInit(&zstrm, 9) == Z_OK) {
        printf(" |- zlib [OK]\n");

    } else {
        printf(" |- zlib [ERROR]\n");
        ret = 1;
    }


    // Test libpng ------------------------------------------------------------
    // ------------------------------------------------------------------------
    png_structp png_ptr;
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr != NULL) {
        printf(" |- libpng [OK]\n");

    } else {
        printf(" |- libpng [ERROR]\n");
        ret = 2;
    }


    // Test lua ---------------------------------------------------------------
    // ------------------------------------------------------------------------
    lua_State *L = luaL_newstate();
    if (L != NULL) {
        luaL_openlibs(L);
        printf(" |- lua [OK]\n");
        
    } else {
        printf(" |- lua [ERROR]\n");
        ret = 3;
    }


    // Test GLFW --------------------------------------------------------------
    // ------------------------------------------------------------------------
    GLFWwindow* window;
    const char *glfwVersion = glfwGetVersionString();
    if (glfwInit()) {
        printf(" |- glfw (%s) [OK]\n", glfwVersion);
        
    } else {
        printf(" |- glfw (%s) [ERROR]\n", glfwVersion);
        ret = 4;
    }

    // Test portaudio ---------------------------------------------------------
    // ------------------------------------------------------------------------
    const char *paVersion = Pa_GetVersionText();
    if (Pa_Initialize() == paNoError) {

        if (Pa_Terminate() == paNoError) {
            printf(" |- portaudio (%s [OK]\n", paVersion);

        } else {
            printf(" |- portaudio (%s) [ERROR]\n", paVersion);
            ret = 5;
        }
        
    } else {
        printf(" |- portaudio (%s) [ERROR]\n", paVersion);
        ret = 5;
    }

    if (ret == 0) {
        printf(" \\- Success!\n");

    } else {
        printf(" \\- Failure\n");
    }

    return ret;

}

