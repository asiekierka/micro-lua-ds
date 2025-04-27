/*
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ds_color_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "vars.h"

static int color_new(lua_State *L){
    int r = (int)luaL_checknumber(L, 1);
    int g = (int)luaL_checknumber(L, 2);
    int b = (int)luaL_checknumber(L, 3);
    assert(L, r>=0 && r<=31, "Red mask must be between 0 and 31")
    assert(L, g>=0 && g<=31, "Green mask must be between 0 and 31")
    assert(L, b>=0 && b<=31, "Blue mask must be between 0 and 31")
    lua_pushnumber(L, RGB15(r, g, b));
    return 1;
}

static int color_new256(lua_State *L) {
    int r = (int)luaL_checknumber(L, 1);
    int g = (int)luaL_checknumber(L, 2);
    int b = (int)luaL_checknumber(L, 3);
    assert(L, r>=0 && r<=255, "Red mask must be between 0 and 255");
    assert(L, g>=0 && g<=255, "Green mask must be between 0 and 255");
    assert(L, b>=0 && b<=255, "Blue mask must be between 0 and 255");
    r = floor(r * 31 / 255);
    g = floor(g * 31 / 255);
    b = floor(b * 31 / 255);
    lua_pushnumber(L, RGB15(r, g, b));
    return 1;
}

static const luaL_Reg colorlib[] = {
    {"new", color_new},
    {"new256", color_new256},
    {NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_color(lua_State *L) {
    luaL_register(L, LUA_COLORLIBNAME, colorlib);
    return 1;
}
