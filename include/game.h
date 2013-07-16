#ifndef GAME_H
#define GAME_H

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

#endif

