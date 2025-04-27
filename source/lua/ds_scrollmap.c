/*
*
*/

#include <stdlib.h>
#include <stdio.h>

#define ds_scrollmap_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <math.h>
#include "lobject.h"
#include "ltable.h"

#include <ulib/ulib.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <string.h>
#include <math.h>

#include "vars.h"

static int scrollmap_new(lua_State *L){
    UL_IMAGE * image = lua_touserdata(L, 1);
    char * filename = (char *)luaL_checkstring(L, 2);
    int width = (int)luaL_checknumber(L, 3);
    int height = (int)luaL_checknumber(L, 4);
    int tileWidth = (int)luaL_checknumber(L, 5);
    int tileHeight = (int)luaL_checknumber(L, 6);
    unsigned short * table;
    table = malloc(width*height*sizeof(unsigned short));
    // Load map from file
    int filesize;
    struct stat file_status;
    if(stat(filename, &file_status) != 0){
        luaL_error(L, "Unable to load %s", filename);
    }
    filesize = file_status.st_size;
    FILE * f = fopen(filename, "r");
    char * buffer = (char *)malloc(sizeof(char)*filesize);
    fread(buffer, 1, filesize, f);
    fclose(f);
    // (Now buffer contains the map file)
    int counter = 0;
    char * token;
    unsigned short tile;
    const char delimiters[] = "|";
    token = strtok(buffer, delimiters);
    tile = (unsigned short)atof(token);
    table[counter] = tile;
    counter++;
    while(token != NULL){
        token = strtok(NULL, delimiters);
        if(token != NULL){
            tile = (unsigned short)atof(token);
            table[counter] = tile;
        }
        counter++;
    }
    free(buffer);
    UL_MAP * map = ulCreateMap(image, (void*)table, tileWidth, tileHeight, width, height, UL_MF_U16);
    lua_pushlightuserdata(L, map);
    return 1;
}

static int scrollmap_destroy(lua_State *L){
    UL_MAP * map = lua_touserdata(L, 1);
    ulDeleteMap(map);
    return 0;
}

static int scrollmap_draw(lua_State *L){
    int screen = (int)luaL_checknumber(L, 1);
    UL_MAP * map = lua_touserdata(L, 2);
    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
        ulDrawMap(map);
    }
    return 0;
}

static int scrollmap_scroll(lua_State *L){
    UL_MAP * map = lua_touserdata(L, 1);
    int x = (int)luaL_checknumber(L, 2);
    int y = (int)luaL_checknumber(L, 3);
    map->scrollX = x;
    map->scrollY = y;
    return 0;
}

static int scrollmap_setTile(lua_State *L){
    UL_MAP * map = lua_touserdata(L, 1);
    int x = (int) luaL_checknumber(L, 2);
    int y = (int) luaL_checknumber(L, 3);
    int tile = (int) luaL_checknumber(L, 4);
    unsigned short * t = map->map;
    t[y*map->mapSizeX+x] = tile;
    return 0;
}

static int scrollmap_getTile(lua_State *L){
    UL_MAP * map = lua_touserdata(L, 1);
    int x = (int) luaL_checknumber(L, 2);
    int y = (int) luaL_checknumber(L, 3);
    unsigned short * t = map->map;
    unsigned short tile = t[y*map->mapSizeX+x];
    lua_pushnumber(L, tile);
    return 1;
}

static const luaL_Reg scrollmaplib[] = {
    {"new", scrollmap_new},
    {"destroy", scrollmap_destroy},
    {"draw", scrollmap_draw},
    {"scroll", scrollmap_scroll},
    {"getTile", scrollmap_getTile},
    {"setTile", scrollmap_setTile},
    {NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_scrollmap(lua_State *L) {
    luaL_register(L, LUA_SCROLLMAPLIBNAME, scrollmaplib);
    return 1;
}
