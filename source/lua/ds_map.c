/*
*
*/

#include <stdlib.h>
#include <stdio.h>

#define ds_map_c
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

void blitTile(int x, int y, UL_IMAGE * img, int sourcex, int sourcey, int width, int height){
    ulSetImageTile(img, sourcex, sourcey, sourcex+width, sourcey+height);
    ulDrawImageXY(img, x, y);
    ulResetImageTile(img);
}

static int map_new(lua_State *L){
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
    Map * map;
    map = malloc(sizeof(image) + sizeof(table) + 8*sizeof(int));
    map->width = width;
    map->height = height;
    map->tileWidth = tileWidth;
    map->tileHeight = tileHeight;
    map->scrollX = 0;
    map->scrollY = 0;
    map->spaceX = 0;
    map->spaceY = 0;
    map->image = image;
    map->table = table;
    lua_pushlightuserdata(L, map);
    return 1;
}

static int map_draw(lua_State *L){
    int screen = (int)luaL_checknumber(L, 1);
    Map * map = lua_touserdata(L, 2);
    int x = (int)luaL_checknumber(L, 3);
    int y = (int)luaL_checknumber(L, 4);
    int width = (int)luaL_checknumber(L, 5);
    int height = (int)luaL_checknumber(L, 6);
    if(!((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd()))){
        return 0;
    }
    int i, j;
    int xx, yy;
    unsigned short tile;
    /*double a, b, c, d;
    a = map->scrollX;
    b = map->scrollY;
    c = map->tileWidth;
    d = map->tileHeight;
    int startx = ceil(a / c); // startx in tile
    int starty = floor(b / d); // starty in tile
    int restx = fmod(a, c);
    int resty = fmod(b, d);
    for(i=starty-1;i<starty-1+pixelHeight/map->tileHeight+1;i++){ // Y
        for(j=startx-1;j<startx-1+pixelWidth/map->tileWidth+1;j++){ // X
            tile = map->table[i*map->height+j];
            yy = floor(tile / (map->image->sizeX / map->tileWidth));
            xx = tile - (yy * (map->image->sizeX / map->tileWidth));
            if(j == startx-1) decalex = restx;
            else decalex = 0;
            if(i == starty-1) decaley = resty;
            else decaley = 0;
            if(j == pixelWidth/map->tileWidth) decalex2 = map->tileWidth - restx;
            else decalex2 = 0;
            if(i == pixelHeight/map->tileHeight) decaley2 = map->tileHeight - resty;
            else decaley2 = 0;
            blitTile(x+j*map->tileWidth, y+i*map->tileHeight,
                map->image, xx * map->tileWidth, yy * map->tileHeight,
                map->tileWidth, map->tileHeight, decalex, decaley, decalex2, decaley2);
        }
    }*/
    int spaceX, spaceY;
    for(i=map->scrollY;i<map->scrollY+height;i++){ // Y
        for(j=map->scrollX;j<map->scrollX+width;j++){ // X
            tile = map->table[i*map->height+j];
            yy = floor(tile / (map->image->sizeX / map->tileWidth));
            xx = tile - (yy * (map->image->sizeX / map->tileWidth));
            spaceX = 0;
            spaceY = 0;
            if(j != map->scrollX) spaceX = map->spaceX;
            if(i != map->scrollY) spaceY = map->spaceY;
            if(i >= 0 && i < map->height && j >= 0 && j < map->width){
                blitTile(x+(j-map->scrollX)*(map->tileWidth+spaceX), y+(i-map->scrollY)*(map->tileHeight+spaceY),
                    map->image, xx * map->tileWidth, yy * map->tileHeight,
                    map->tileWidth, map->tileHeight);
            }
        }
    }
    return 0;
}

static int map_destroy(lua_State *L){
    Map * map = lua_touserdata(L, 1);
    free(map->table);
    free(map);
    return 0;
}

static int map_scroll(lua_State *L){
    Map * map = lua_touserdata(L, 1);
    int x = (int)luaL_checknumber(L, 2);
    int y = (int)luaL_checknumber(L, 3);
    map->scrollX = x;
    map->scrollY = y;
    return 0;
}

static int map_space(lua_State *L){
    Map * map = lua_touserdata(L, 1);
    int x = (int) luaL_checknumber(L, 2);
    int y = (int) luaL_checknumber(L, 3);
    map->spaceX = x;
    map->spaceY = y;
    return 0;
}

static int map_setTile(lua_State *L){
    Map * map = lua_touserdata(L, 1);
    int x = (int) luaL_checknumber(L, 2);
    int y = (int) luaL_checknumber(L, 3);
    int tile = (int) luaL_checknumber(L, 4);
    map->table[y*map->width+x] = tile;
    return 0;
}

static int map_getTile(lua_State *L){
    Map * map = lua_touserdata(L, 1);
    int x = (int) luaL_checknumber(L, 2);
    int y = (int) luaL_checknumber(L, 3);
    unsigned short tile = map->table[y*map->width+x];
    lua_pushnumber(L, tile);
    return 1;
}

static const luaL_Reg maplib[] = {
    {"new", map_new},
    {"destroy", map_destroy},
    {"draw", map_draw},
    {"scroll", map_scroll},
    {"space", map_space},
    {"setTile", map_setTile},
    {"getTile", map_getTile},
    {NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_map(lua_State *L) {
    luaL_register(L, LUA_MAPLIBNAME, maplib);
    return 1;
}
