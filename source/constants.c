/*  
    Copyright 2008-2013 Risike, Reylak, MicroLua's developers
    
    This file is part of MicroLua.

    MicroLua is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MicroLua is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MicroLua.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "lua/ds_lib.h"
#include "constants.h"

bool nitroFsAvailable;

void uLua_pushConstants(lua_State *L) {
    lua_pushliteral(L, ULUA_VERSION);
    lua_setglobal(L, "ULUA_VERSION");
    
    lua_pushliteral(L, ULUA_DIR);
    lua_setglobal(L, "ULUA_DIR");
    lua_pushliteral(L, ULUA_SCRIPTS);
    lua_setglobal(L, "ULUA_SCRIPTS");
    lua_pushliteral(L, ULUA_LIBS);
    lua_setglobal(L, "ULUA_LIBS");
    lua_pushliteral(L, ULUA_BOOT_FILE);
    lua_setglobal(L, "ULUA_BOOT_FILE");
    lua_pushliteral(L, ULUA_BOOT_FULLPATH);
    lua_setglobal(L, "ULUA_BOOT_FULLPATH");
    
    lua_pushnumber(L, SCREEN_WIDTH);
    lua_setglobal(L, "SCREEN_WIDTH");
    lua_pushnumber(L, SCREEN_HEIGHT);
    lua_setglobal(L, "SCREEN_HEIGHT");
    
    lua_pushnumber(L, ULUA_RAM);
    lua_setglobal(L, "RAM");
    lua_pushnumber(L, ULUA_VRAM);
    lua_setglobal(L, "VRAM");
    
    lua_pushnumber(L, SCREEN_DOWN);
    lua_setglobal(L, "SCREEN_DOWN");
    lua_pushnumber(L, SCREEN_UP);
    lua_setglobal(L, "SCREEN_UP");
    
    lua_pushnumber(L, ALPHA_RESET);
    lua_setglobal(L, "ALPHA_RESET");
	
	lua_pushnumber(L, LED_ON);
	lua_setglobal(L, "LED_ON");
	lua_pushnumber(L, LED_SLEEP);
	lua_setglobal(L, "LED_SLEEP");
	lua_pushnumber(L, LED_BLINK);
	lua_setglobal(L, "LED_BLINK");
    
    lua_pushnumber(L, ATTR_X1);
    lua_setglobal(L, "ATTR_X1");
    lua_pushnumber(L, ATTR_Y1);
    lua_setglobal(L, "ATTR_Y1");
    lua_pushnumber(L, ATTR_X2);
    lua_setglobal(L, "ATTR_X2");
    lua_pushnumber(L, ATTR_Y2);
    lua_setglobal(L, "ATTR_Y2");
    lua_pushnumber(L, ATTR_X3);
    lua_setglobal(L, "ATTR_X3");
    lua_pushnumber(L, ATTR_Y3);
    lua_setglobal(L, "ATTR_Y3");
    lua_pushnumber(L, ATTR_COLOR);
    lua_setglobal(L, "ATTR_COLOR");
    lua_pushnumber(L, ATTR_COLOR1);
    lua_setglobal(L, "ATTR_COLOR1");
    lua_pushnumber(L, ATTR_COLOR2);
    lua_setglobal(L, "ATTR_COLOR2");
    lua_pushnumber(L, ATTR_COLOR3);
    lua_setglobal(L, "ATTR_COLOR3");
    lua_pushnumber(L, ATTR_COLOR4);
    lua_setglobal(L, "ATTR_COLOR4");
    lua_pushnumber(L, ATTR_TEXT);
    lua_setglobal(L, "ATTR_TEXT");
    lua_pushnumber(L, ATTR_VISIBLE);
    lua_setglobal(L, "ATTR_VISIBLE");
    lua_pushnumber(L, ATTR_FONT);
    lua_setglobal(L, "ATTR_FONT");
    lua_pushnumber(L, ATTR_IMAGE);
    lua_setglobal(L, "ATTR_IMAGE");
    
    lua_pushnumber(L, PLAY_LOOP);
    lua_setglobal(L, "PLAY_LOOP");
    lua_pushnumber(L, PLAY_ONCE);
    lua_setglobal(L, "PLAY_ONCE");
    
    // Stylus table
    
    lua_newtable(L);
	lua_pushstring(L,"X");
	lua_pushnumber(L,-1);
	lua_settable(L,-3);
	lua_pushstring(L,"Y");
	lua_pushnumber(L,-1);
	lua_settable(L,-3);
	lua_pushstring(L,"held");
	lua_pushboolean(L,0);
	lua_settable(L,-3);
	lua_pushstring(L,"released");
	lua_pushboolean(L,0);
	lua_settable(L,-3);
	lua_pushstring(L,"doubleClick");
	lua_pushboolean(L,0);
	lua_settable(L,-3);
	lua_pushstring(L,"deltaX");
	lua_pushnumber(L,-1);
	lua_settable(L,-3);
	lua_pushstring(L,"deltaY");
	lua_pushnumber(L,-1);
	lua_settable(L,-3);
	lua_pushstring(L,"newPress");
	lua_pushboolean(L,0);
	lua_settable(L,-3);
	lua_setglobal(L,"Stylus");
	
	// Keys table
	
	lua_newtable(L);
	lua_newtable(L);
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"A");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"B");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"X");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Y");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"L");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"R");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Start");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Select");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Up");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Down");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Left");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Right");
	lua_setfield(L,-2,"held");
	lua_newtable(L);
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"A");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"B");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"X");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Y");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"L");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"R");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Start");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Select");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Up");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Down");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Left");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Right");
	lua_setfield(L,-2,"released");
	lua_newtable(L);
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"A");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"B");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"X");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Y");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"L");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"R");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Start");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Select");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Up");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Down");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Left");
	lua_pushboolean(L,0);
	lua_setfield(L,-2,"Right");
	lua_setfield(L,-2,"newPress");
	lua_setglobal(L,"Keys");
	
    // Availability of EFS
    lua_getglobal(L, LUA_SYSTEMLIBNAME);
    lua_pushboolean(L, nitroFsAvailable);
    lua_setfield(L, -2, "EFS");
}
