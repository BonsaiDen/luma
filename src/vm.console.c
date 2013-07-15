#include "../include/support.h"
#include "../include/vm.console.h"


// Internal -------------------------------------------------------------------
// ----------------------------------------------------------------------------
void internal_print(lua_State *L, const int i,
    const char *cr, const char *cn, const char *cs,
    const char *cnil, const char *cb, const char *co, const bool wrapString) {
        
    unsigned int length = 0;
    unsigned int count = 0;
    unsigned int index = 0;
    bool initial = true;

    if (lua_isnumber(L, i)) {
        fprintf(stdout, "%s%s%s", cn, lua_tolstring(L, i, &length), cr);

    } else if (lua_isstring(L, i)) {
        if (wrapString) {
            fprintf(stdout, "%s'%s'%s", co, lua_tolstring(L, i, &length), cr);

        } else {
            fprintf(stdout, "%s%s%s", cs, lua_tolstring(L, i, &length), cr);
        }

    } else if (lua_isnil(L, i)) {
        fprintf(stdout, "%snil%s", cnil, cr);

    } else if (lua_isboolean(L, i)) {

        if (lua_toboolean(L, i)) {
            fprintf(stdout, "%strue%s", cb, cr);

        } else {
            fprintf(stdout, "%sfalse%s", cb, cr);
        }
        
    } else if (lua_istable(L, i)) {

        lua_len(L, i);
        count = lua_tointeger(L, -1);
        fprintf(stdout, "%s{%s ", co, cr);

        lua_pushnil(L); 
        while(lua_next(L, i) != 0) {

            if (!initial) {
                fprintf(stdout, ", ");

            } else {
                initial = false;
            }

            if (lua_isnumber(L, -2) && lua_isstring(L, -2)) {
                fprintf(stdout, "%s%d%s = ", cn, ++index, cr);
                
            } else {
                fprintf(stdout, "%s%s%s = ", cs, lua_tolstring(L, -2, &length), cr);
            }

            internal_print(L, lua_gettop(L), cr, cn, cs, cnil, cb, co, true);
            lua_pop(L, 1);

        }
        fprintf(stdout, " %s}(#%s%d%s)%s", co, cn, count, co, cr);

        lua_pop(L, 1);

    }

}

int internal_log(lua_State *L, const char *name,
    const char *cr, const char *cn, const char *cs,
    const char *cnil, const char *cb, const char *co) {

    const int n = lua_gettop(L);
    fprintf(stdout, "[console:%s]", name);

    int i = 0;
    for (i = 1; i <= n; i++) {
        fprintf(stdout, " ");
        internal_print(L, i, cr, cn, cs, cnil, cb, co, false);
    }
    fprintf(stdout, "\n");

    return 0;

}


// API ------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int console_log(lua_State *L) {
    return internal_log(
        L, "log", 
        "\x1b[0m", "\x1b[34m", "\x1b[37m", "\x1b[31m", "\x1b[31m", "\x1b[32m"
    );
}

int console_info(lua_State *L) {
    return internal_log(
        L, "info", 
        "\x1b[0m", "\x1b[34m", "\x1b[34m", "\x1b[34m", "\x1b[34m", "\x1b[34m"
    );
}

int console_warn(lua_State *L) {
    return internal_log(
        L, "warn", 
        "\x1b[0m", "\x1b[33m", "\x1b[33m", "\x1b[33m", "\x1b[33m", "\x1b[33m"
    );
}

int console_error(lua_State *L) {
    return internal_log(
        L, "error", 
        "\x1b[0m", "\x1b[31m", "\x1b[31m", "\x1b[31m", "\x1b[31m", "\x1b[31m"
    );
}

