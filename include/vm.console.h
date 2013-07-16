#ifndef LUMA_VM_CONSOLE_H
#define LUMA_VM_CONSOLE_H

#include "../deps/include/lua.h"
#include "../deps/include/lualib.h"
#include "../deps/include/lauxlib.h"


// API ------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int console_log(lua_State *L);
int console_info(lua_State *L);
int console_warn(lua_State *L);
int console_error(lua_State *L);

#endif

