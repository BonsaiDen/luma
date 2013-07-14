#include "../include/game.h"
#include "../include/vm.h"
#include "../include/io.h"


// API ------------------------------------------------------------------------
// ----------------------------------------------------------------------------
#define assert_empty_stack() assert(lua_gettop(gLuaState) == 1);

int internal_handle_error(lua_State *L);
int internal_console_log(lua_State *L);
bool internal_require(const char *filename, const bool returnModule);

bool vm_init() {

    gLuaState = luaL_newstate();
    if (gLuaState != NULL) {

        // Load lua support libraries
        luaL_openlibs(gLuaState);

        // Override lua's builtin require function and set the error handler
        lua_getglobal(gLuaState, "_G"); // +1
        lua_pushcfunction(gLuaState, luax_require); // +1
        lua_setfield(gLuaState, -2, "require"); // -1
        lua_pop(gLuaState, 1); // -1
        lua_pushcfunction(gLuaState, &internal_handle_error); // +1 = 0

        // Setup console
        vm_object_create();
        vm_field_set_function("log", internal_console_log);
        vm_global_set("console");

        assert_empty_stack();

        vm_info("init success");
        return true;
        
    } else {
        vm_info("init failed to create lua state");
        return false;
    }

}

bool vm_require(const char *filename) {
    return internal_require(filename, false);
}

bool vm_update() {
    vm_info("update");
    return true;
}

bool vm_shutdown() {

    if (gLuaState != NULL) {
        lua_close(gLuaState);
        vm_info("shutdown success");
        return true;
        
    } else {
        vm_info("shutdown failed");
        return false;
    }
    
}

void vm_api() {

    vm_object_create();
    vm_field_set_string("name", "ivo");
    vm_global_set("user");

    vm_global_get("user");
    vm_info("user name is %s", vm_field_get_string("name"));
    vm_rewind();

}



// Abstractions ---------------------------------------------------------------
// ----------------------------------------------------------------------------
void vm_rewind() {
    const int count = lua_gettop(gLuaState) - 1;
    if (count > 0) {
        vm_info("dropping %d stack entries", count);
        lua_pop(gLuaState, count);
    }
    assert_empty_stack();
}

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


// Extensions -----------------------------------------------------------------
// ----------------------------------------------------------------------------
int internal_handle_error(lua_State *L) {
    
    const char *err = lua_tostring(L, -1); // = 1
    lua_getglobal(L, "debug"); // +1
    lua_getfield(L, -1, "traceback"); // +1
    lua_remove(L, -2); // - 1
    lua_call(L, 0, 1); // +1
    vm_error("%s\n - %s\n", lua_tostring(L, -1), err);
    lua_pop(L, 1); // -1 = 1

    return 0;

}

int internal_console_log(lua_State *L) {

    const int n = lua_gettop(L);

    int i = 0;
    unsigned int length = 0;

    fprintf(stdout, "[console:log]");
    for (i = 1; i <= n; i++) {

        fprintf(stdout, " ");

        if (lua_isnumber(L, i)) {
            fprintf(stdout, "\x1b[34m%s\x1b[0m", lua_tolstring(L, i, &length));

        } else if (lua_isstring(L, i)) {
            fprintf(stdout, "\x1b[37m%s\x1b[0m", lua_tolstring(L, i, &length));

        } else if (lua_isnil(L, i)) {
            fprintf(stdout, "\x1b[31mnil\x1b[0m");

        } else if (lua_isboolean(L, i)) {

            if (lua_toboolean(L, i)) {
                fprintf(stdout, "\x1b[31mtrue\x1b[0m");

            } else {
                fprintf(stdout, "\x1b[31mfalse\x1b[0m");
            }
            
        } else if (lua_istable(L, i)) {
            length = lua_rawlen(L, i);
            fprintf(stdout, "\x1b[32m{Object #%d}\x1b[0m", length);
        }

    }

    fprintf(stdout, "\n");

    return 0;

}

bool internal_require(const char *filename, const bool returnModule) {

    char *buf;
    unsigned int len;

    buf = io_load_resource(filename, &len);
    if (buf != NULL) {

        // Leaving the return value of the module on the stack (if wanted)
        vm_info("script '%s' loaded", filename);
        luaL_loadbuffer(gLuaState, buf, len, filename); //
        lua_pcall(gLuaState, 0, 1, 0);  // +1 = 2

        if (!returnModule) {
            lua_pop(gLuaState, 1); // -1 = 1
            assert_empty_stack();
        }

        free(buf);
        vm_info("script '%s' executed", filename);

        return true;

    } else {
        vm_error("script '%s' not loaded", filename);
        return false;
    }

}

int luax_require(lua_State *L) {

    const char *filename = luaL_checkstring(L, 1);
    char *str = (char*)calloc(strlen(filename) + 5, sizeof(char));
    strcat(str, filename);
    strcat(str, ".lua");
    internal_require(str, true);
    free(str);
    return 1;

}

bool luax_optboolean(lua_State * L, const int idx, const bool b) {

    if (lua_isboolean(L, idx) == 1) {
        return (lua_toboolean(L, idx) == 1 ? true : false);

    } else {
        return b;
    }

}

