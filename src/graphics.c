#include "../include/graphics.h"


// Variables ------------------------------------------------------------------
// ----------------------------------------------------------------------------
GLFWwindow *window;


// API ------------------------------------------------------------------------
// ----------------------------------------------------------------------------
static void internal_error_callback(int error, const char *description) {
    graphics_error("%s", description);
}

bool graphics_init(const int width, const int height, const char *title) {

    glfwSetErrorCallback(internal_error_callback);
    if (!glfwInit()) {
        graphics_info("glfw init failed");
        return false;
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(width, height, title, NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Setup orthographic 2D view
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);

    graphics_info("init");

    return true;

}

bool graphics_update(void(*callback)(const double time), 
                     void(*key_callback)(GLFWwindow* window, int key, 
                                        int scancode, int action, int mods)) {

    double time = 0;
    double offset = 0;

    glfwSetTime(0);
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {

        time += glfwGetTime() - offset;
        offset = glfwGetTime();
        callback(time);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    graphics_info("window closed");

    return true;

}

bool graphics_shutdown() {
    glfwDestroyWindow(window);
    glfwTerminate();
    graphics_info("shutdown success");
    return true;
}

