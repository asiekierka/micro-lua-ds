/*
*
*/

#include <stdlib.h>
#include <stdio.h>

#define ds_font_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <ulib/ulib.h>
#include <sys/stat.h>
#include <sys/dir.h>

#include "vars.h"

static int font_load(lua_State *L){
    char * filename = (char *)luaL_checkstring(L, 1);
    assert(L, filename != NULL, "Path can't be null");
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
    UL_FONT * font;
    font = ulLoadFontFile((void *)buffer, (int)filesize);
    if(font == NULL){
        luaL_error(L, "Unable to load %s", filename);
        return 0;
    }
    lua_pushlightuserdata(L, font);
    free(buffer);
    return 1;
}

static int font_getStringWidth(lua_State *L){
    UL_FONT * font = lua_touserdata(L, 1);
    assert(L, font != NULL, "Font can't be null");
    char * text = (char *)luaL_checkstring(L, 2);
    assert(L, text != NULL, "Text can't be null");
    if(font != NULL){
        ulSetFont(font);
    }
    int width = ulGetStringWidth(text);
    if(font != NULL){
        ulSetFont(ul_lcdFont);
    }
    lua_pushnumber(L, width);
    return 1;
}

static int font_getCharHeight(lua_State *L){
    UL_FONT * font = lua_touserdata(L, 1);
    assert(L, font != NULL, "Font can't be null");
    lua_pushnumber(L, font->charHeight);
    return 1;
}

static int font_destroy(lua_State *L){
    UL_FONT * font = lua_touserdata(L, 1);
    if(font->img->imgState == UL_STATE_VRAM) ulUnrealizeImage(font->img);
    ulDeleteImage(font->img);
    free(font);
    return 0;
}

static const luaL_Reg fontlib[] = {
    {"load", font_load},
    {"getStringWidth", font_getStringWidth},
    {"getCharHeight", font_getCharHeight},
    {"destroy", font_destroy},
    {NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_font(lua_State *L) {
    luaL_register(L, LUA_FONTLIBNAME, fontlib);
    return 1;
}
