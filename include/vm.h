#ifndef LUMA_VM_H
#define LUMA_VM_H

#include "../deps/include/lua.h"
#include "../deps/include/lualib.h"
#include "../deps/include/lauxlib.h"


// Logging --------------------------------------------------------------------
// ----------------------------------------------------------------------------
#define vm_log(format, args...) _log(vm, format, ##args)
#define vm_info(format, args...) _info(vm, format, ##args) 
#define vm_warn(format, args...) _warn(vm, format, ##args) 
#define vm_error(format, args...) _error(vm, format, ##args)


// Structures -----------------------------------------------------------------
// ----------------------------------------------------------------------------
lua_State *gLuaState;


// API ------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool vm_init();
bool vm_require(const char *filename);
bool vm_update();
bool vm_shutdown();


// Extensions -----------------------------------------------------------------
// ----------------------------------------------------------------------------
int luax_require(lua_State * L);
bool luax_optboolean(lua_State * L, const int idx, const bool b);

#endif

