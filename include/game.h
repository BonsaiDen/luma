#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "support.h"


// Logging --------------------------------------------------------------------
// ----------------------------------------------------------------------------
#define game_log(format, args...) _log(game, format, ##args)
#define game_info(format, args...) _info(game, format, ##args) 
#define game_warn(format, args...) _warn(game, format, ##args) 
#define game_error(format, args...) _error(game, format, ##args)


// Variables ------------------------------------------------------------------
// ----------------------------------------------------------------------------


// Structures -----------------------------------------------------------------
// ----------------------------------------------------------------------------


// API ------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool game_init();
bool game_update();
bool game_shutdown();
void game_api();

void vm_rewind();
void vm_global_set(const char *name);
void vm_global_get(const char *name);
void vm_object_create();
void vm_field_set(const char *name);
void vm_field_get();
void vm_field_set_nil(const char *field);
void vm_field_set_function(const char *field, void *func);
void vm_field_set_boolean(const char *field, bool b);
bool vm_field_get_boolean(const char *field);
void vm_field_set_string(const char *field, const char *string);
const char *vm_field_get_string(const char *field);
void vm_field_set_integer(const char *field, const int i);
int vm_field_get_integer(const char *field);
void vm_field_set_double(const char *field, const double d);
double vm_field_get_double(const char *field);

#endif

