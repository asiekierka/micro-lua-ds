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

#include <stdio.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <ulib/ulib.h>
#include <fat.h>
#include <filesystem.h>
#include <unistd.h>

#include <dswifi9.h>
#include <netinet/in.h>

#include "constants.h"
#include "lua/ds_lib.h"


void print_error(const char *text)
{
	while (1)
	{
		ulStartDrawing2D();
		ulDrawTextBox(0,0,256,192,text,0);
		ulEndDrawing();
		ulSyncFrame();
	}
}

int main()
{
    // Initialization of µlibrary
    ulInit(UL_INIT_ALL);
    ulInitGfx();
    ulInitText();

    // Sucks banks C-D + drops at 30 fps
    ulInitDualScreenMode();
    
    // Banks A-B for textures VRAM
    ulSetTexVramParameters(UL_BANK_A | UL_BANK_B, VRAM_A, 256 << 10);
    
    // Banks F-G for palettes VRAM
    ulSetTexPalVramParameters(UL_BANK_F | UL_BANK_G, VRAM_F, 32 << 10);
    
    // Use bright pink as a transparent color
    ulSetTransparentColor(RGB15(31, 0, 31));
    
    /*  Initialization of libfat and NitroFS
    *   According to the macro EFS defined upon compilation,
    *   MicroLua will init FAT alone, or EFS and FAT */
    bool fatInitOk = fatInitDefault();
    bool nitrofsInitOk = nitroFSInit(NULL);
    if (!fatInitOk && !nitrofsInitOk) {
        print_error("Failed to initialize FAT library");
        return 1;
    }
    
    timerStart(TIMER_ID, ClockDivider_1024, 0, NULL);
    
    struct lua_State *l = luaL_newstate();
    if (!l) {
        print_error("\n\n\tFailed to create a Lua state - Push A to Exit\n");
        return 1;
    }
    
    luaL_openlibs(l);
    luaL_openDSlibs(l);
    uLua_pushConstants(l);
    
    // First we try to load from fat:
    chdir("fat:/");
    if (luaL_loadfile(l, ULUA_BOOT_FULLPATH)) {
        if (luaL_loadfile(l, ULUA_LIBS"libs.lua")) {
            if (luaL_loadfile(l, "nitro:"ULUA_BOOT_FULLPATH)) {     // Then from NitroFS
                if (luaL_loadfile(l, "nitro:/"ULUA_LIBS"libs.lua")) {
                    char text[256];
                    sprintf(text,"Error Occured: Couldn't open (nitro:/)%s\n", ULUA_BOOT_FULLPATH);
                    print_error(text);
                    return 1;
                }
            }
            // Here we only have NitroFS, so we set it as default device
            chdir("nitro:/");
        }
    }
    
    if(lua_pcall(l,0,0,0)) {
        print_error(lua_tostring(l, -1));
        lua_pop(l, 1);
        return 1;
    }
    
    lua_close(l);

    return 0;
}
