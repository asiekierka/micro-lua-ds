/*
*
*/

#include <stdlib.h>
#include <stdio.h>

#define ds_rumble_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <nds.h>

#include "vars.h"

static int rumble_isInserted(lua_State *L){
    lua_pushboolean(L, isRumbleInserted());
    return 1;
}

static int rumble_set(lua_State *L){
    setRumble(lua_toboolean(L, 1) ? rumbleGetMaxRawStrength() : 0);
    return 0;
}

static const luaL_Reg rumblelib[] = {
    {"isInserted", rumble_isInserted},
    {"set", rumble_set},
    {NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_rumble(lua_State *L) {
    luaL_register(L, LUA_RUMBLELIBNAME, rumblelib);
    return 1;
}
