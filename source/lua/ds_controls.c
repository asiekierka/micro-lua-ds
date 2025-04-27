/*
*
*/

#include <stdlib.h>
#include <stdio.h>

#define ds_controls_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <ulib/ulib.h>

#include "vars.h"

static int controls_read(lua_State *L){
	ulReadKeys(0);
	// Stylus table
	lua_newtable(L);
	lua_pushstring(L,"X");
	lua_pushnumber(L,ul_keys.touch.x);
	lua_settable(L,-3);
	lua_pushstring(L,"Y");
	lua_pushnumber(L,ul_keys.touch.y);
	lua_settable(L,-3);
	lua_pushstring(L,"held");
	lua_pushboolean(L,ul_keys.touch.held);
	lua_settable(L,-3);
	lua_pushstring(L,"released");
	lua_pushboolean(L,ul_keys.touch.released);
	lua_settable(L,-3);
	lua_pushstring(L,"doubleClick");
	lua_pushboolean(L,ul_keys.touch.doubleClick);
	lua_settable(L,-3);
	lua_pushstring(L,"deltaX");
	lua_pushnumber(L,ul_keys.touch.deltaX);
	lua_settable(L,-3);
	lua_pushstring(L,"deltaY");
	lua_pushnumber(L,ul_keys.touch.deltaY);
	lua_settable(L,-3);
	lua_pushstring(L,"newPress");
	lua_pushboolean(L,ul_keys.touch.click);
	lua_settable(L,-3);
	lua_setglobal(L,"Stylus");
	// Keys table
	lua_newtable(L);
	lua_newtable(L);
	lua_pushboolean(L,ul_keys.held.A);
	lua_setfield(L,-2,"A");
	lua_pushboolean(L,ul_keys.held.B);
	lua_setfield(L,-2,"B");
	lua_pushboolean(L,ul_keys.held.X);
	lua_setfield(L,-2,"X");
	lua_pushboolean(L,ul_keys.held.Y);
	lua_setfield(L,-2,"Y");
	lua_pushboolean(L,ul_keys.held.L);
	lua_setfield(L,-2,"L");
	lua_pushboolean(L,ul_keys.held.R);
	lua_setfield(L,-2,"R");
	lua_pushboolean(L,ul_keys.held.start);
	lua_setfield(L,-2,"Start");
	lua_pushboolean(L,ul_keys.held.select);
	lua_setfield(L,-2,"Select");
	lua_pushboolean(L,ul_keys.held.up);
	lua_setfield(L,-2,"Up");
	lua_pushboolean(L,ul_keys.held.down);
	lua_setfield(L,-2,"Down");
	lua_pushboolean(L,ul_keys.held.left);
	lua_setfield(L,-2,"Left");
	lua_pushboolean(L,ul_keys.held.right);
	lua_setfield(L,-2,"Right");
	lua_pushboolean(L,ul_keys.held.lid);
	lua_setfield(L, -2, "Lid");
	lua_setfield(L,-2,"held");
	lua_newtable(L);
	lua_pushboolean(L,ul_keys.released.A);
	lua_setfield(L,-2,"A");
	lua_pushboolean(L,ul_keys.released.B);
	lua_setfield(L,-2,"B");
	lua_pushboolean(L,ul_keys.released.X);
	lua_setfield(L,-2,"X");
	lua_pushboolean(L,ul_keys.released.Y);
	lua_setfield(L,-2,"Y");
	lua_pushboolean(L,ul_keys.released.L);
	lua_setfield(L,-2,"L");
	lua_pushboolean(L,ul_keys.released.R);
	lua_setfield(L,-2,"R");
	lua_pushboolean(L,ul_keys.released.start);
	lua_setfield(L,-2,"Start");
	lua_pushboolean(L,ul_keys.released.select);
	lua_setfield(L,-2,"Select");
	lua_pushboolean(L,ul_keys.released.up);
	lua_setfield(L,-2,"Up");
	lua_pushboolean(L,ul_keys.released.down);
	lua_setfield(L,-2,"Down");
	lua_pushboolean(L,ul_keys.released.left);
	lua_setfield(L,-2,"Left");
	lua_pushboolean(L,ul_keys.released.right);
	lua_setfield(L,-2,"Right");
	lua_pushboolean(L,ul_keys.released.lid);
	lua_setfield(L, -2, "Lid");
	lua_setfield(L,-2,"released");
	lua_newtable(L);
	lua_pushboolean(L,ul_keys.pressed.A);
	lua_setfield(L,-2,"A");
	lua_pushboolean(L,ul_keys.pressed.B);
	lua_setfield(L,-2,"B");
	lua_pushboolean(L,ul_keys.pressed.X);
	lua_setfield(L,-2,"X");
	lua_pushboolean(L,ul_keys.pressed.Y);
	lua_setfield(L,-2,"Y");
	lua_pushboolean(L,ul_keys.pressed.L);
	lua_setfield(L,-2,"L");
	lua_pushboolean(L,ul_keys.pressed.R);
	lua_setfield(L,-2,"R");
	lua_pushboolean(L,ul_keys.pressed.start);
	lua_setfield(L,-2,"Start");
	lua_pushboolean(L,ul_keys.pressed.select);
	lua_setfield(L,-2,"Select");
	lua_pushboolean(L,ul_keys.pressed.up);
	lua_setfield(L,-2,"Up");
	lua_pushboolean(L,ul_keys.pressed.down);
	lua_setfield(L,-2,"Down");
	lua_pushboolean(L,ul_keys.pressed.left);
	lua_setfield(L,-2,"Left");
	lua_pushboolean(L,ul_keys.pressed.right);
	lua_setfield(L,-2,"Right");
	lua_pushboolean(L,ul_keys.pressed.lid);
	lua_setfield(L, -2, "Lid");
	lua_setfield(L,-2,"newPress");
	lua_setglobal(L,"Keys");
	return 0;
}

static int controls_setStylusDblcFreq(lua_State *L){
	char tempo = luaL_checknumber(L,-1);
	assert(L, tempo>0 && tempo<30, "Must be between 0 and 30");
	ulSetTouchpadDoubleClickDelay(tempo);
	return 0;
}



static const luaL_Reg controlslib[] = {
    // Stylus
    {"read", controls_read},
    {"setStylusDblcFreq", controls_setStylusDblcFreq},
    {NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_controls(lua_State *L) {
    luaL_register(L, LUA_CONTROLSLIBNAME, controlslib);
    return 1;
}
