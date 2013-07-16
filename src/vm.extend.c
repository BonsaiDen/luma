#include "../include/support.h"
#include "../include/vm.h"
#include "../include/vm.extend.h"


// Abstractions ---------------------------------------------------------------
// ----------------------------------------------------------------------------
void vm_global_set(const char *name) {
    lua_setglobal(gLuaState, name);
}

void vm_global_get(const char *name) {
    lua_getglobal(gLuaState, name);
}

void vm_object_create() {
    lua_newtable(gLuaState);
}

void vm_field_get(const char *field) {
    lua_getfield(gLuaState, -1, field);
}

void vm_field_set(const char *field) {
    lua_setfield(gLuaState, -2, field);
}

void vm_field_set_nil(const char *field) {
    lua_pushnil(gLuaState);
    lua_setfield(gLuaState, -2, field);
}

void vm_field_set_function(const char *field, void *func) {
    lua_pushcfunction(gLuaState, func);
    lua_setfield(gLuaState, -2, field);
}

void vm_field_set_string(const char *field, const char *string) {
    lua_pushstring(gLuaState, string);
    lua_setfield(gLuaState, -2, field);
}

const char *vm_field_get_string(const char *field) {

    unsigned int length = 0;
    const char *string;

    lua_getfield(gLuaState, -1, field);
    string = lua_tolstring(gLuaState, -1, &length);
    lua_pop(gLuaState, 1);

    return string;

}

void vm_field_set_integer(const char *field, const int i) {
    lua_pushinteger(gLuaState, i);
    lua_setfield(gLuaState, -2, field);
}

int vm_field_get_integer(const char *field) {

    int i = 0;

    lua_getfield(gLuaState, -1, field);
    i = lua_tointeger(gLuaState, -1);
    lua_pop(gLuaState, 1);

    return i;

}

void vm_field_set_double(const char *field, const double d) {
    lua_pushnumber(gLuaState, d);
    lua_setfield(gLuaState, -2, field);
}

double vm_field_get_double(const char *field) {
    
    double d = 0;

    lua_getfield(gLuaState, -1, field);
    d = lua_tonumber(gLuaState, -1);
    lua_pop(gLuaState, 1);

    return d;

}

