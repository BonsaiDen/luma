#ifndef LUMA_GRAPHICS_H
#define LUMA_GRAPHICS_H

#include "../include/support.h"
#include "../deps/include/glfw3.h"


// Logging --------------------------------------------------------------------
// ----------------------------------------------------------------------------
#define graphics_log(format, args...) _log(graphics, format, ##args)
#define graphics_info(format, args...) _info(graphics, format, ##args) 
#define graphics_warn(format, args...) _warn(graphics, format, ##args) 
#define graphics_error(format, args...) _error(graphics, format, ##args)


// API ------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool graphics_init(const int width, const int height, const char *title);
bool graphics_update(void(*game_callback)(const double time), 
                    void(*key_callback)(GLFWwindow* window, int key, 
                                        int scancode, int action, int mods));

bool graphics_shutdown();

#endif

