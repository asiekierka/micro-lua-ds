/*
*
*/

#include <stdlib.h>
#include <stdio.h>

#define ds_image_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <ulib/ulib.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <string.h>

#include "vars.h"

static int image_load(lua_State *L){
    char * filename = (char *)luaL_checkstring(L, 1);
    int dest = (int)luaL_checknumber(L, 2);
    assert(L, filename != NULL, "Path can't be null");
    assert(L, dest == LUA_RAM || dest == LUA_VRAM, "Destination must be RAM or VRAM");
    int filesize = 0;
    struct stat file_status;
    if(stat(filename, &file_status) != 0){
        luaL_error(L, "Unable to load %s", filename);
        return 0;
    }
    filesize = file_status.st_size;
    FILE * f = fopen(filename, "r");
    char * buffer = (char *)malloc(sizeof(char)*filesize);
    if(buffer == NULL) {
        luaL_error(L, "Unable to load %s (out of memory)", filename);
        return 0;
    }
    fread(buffer, 1, filesize, f);
    fclose(f);
    UL_IMAGE * img;
    char * ext = "";
    ext = strlwr(filename+strlen(filename)-4);
    if(!strcmp(ext, ".png")){ // ext == ".png"
        if(!dest) img = ulLoadImageFilePNG((void *)buffer, (int)filesize, UL_IN_RAM, UL_PF_PAL8);
        else img = ulLoadImageFilePNG((void *)buffer, (int)filesize, UL_IN_VRAM, UL_PF_PAL8);
    }else{
        if(!strcmp(ext, ".gif")){ // ext == ".gif"
            if(!dest) img = ulLoadImageFileGIF((void *)buffer, (int)filesize, UL_IN_RAM, UL_PF_PAL8);
            else img = ulLoadImageFileGIF((void *)buffer, (int)filesize, UL_IN_VRAM, UL_PF_PAL8);
        }else{
            if((!strcmp(ext, ".jpg")) || (!strcmp((strlwr(filename+strlen(filename)-5)), ".jpeg"))){ // ext == ".jpg" or ".jpeg"
                if(!dest) img = ulLoadImageFileJPG((void *)buffer, (int)filesize, UL_IN_RAM, UL_PF_5550);
                else img = ulLoadImageFileJPG((void *)buffer, (int)filesize, UL_IN_VRAM, UL_PF_5550);
            }else{ // other file
                luaL_error(L, "Image file must be a .png, .gif, .jpg or .jpeg file");
                img = NULL;
                return 0;
            }
        }
    }
    free(buffer);
    buffer = NULL;
    if(img == NULL){
        luaL_error(L, "Can't load %s perhaps due to a lack of memory.", filename);
        return 0;
        // Si pas d'images chargée l'indiquer en renvoyant nil
        //lua_pushnil(L);
        //return 1;
    }
    lua_pushlightuserdata(L, img);
    return 1;
}

static int image_destroy(lua_State *L){
    UL_IMAGE * img = lua_touserdata(L, 1);
    assert(L, img != NULL, "Bad image ressource");
    if(img->imgState == UL_STATE_VRAM) ulUnrealizeImage(img);
    ulDeleteImage(img);
    return 0;
}

static int image_width(lua_State *L){
    UL_IMAGE * img = lua_touserdata(L, 1);
    assert(L, img != NULL, "Bad image ressource");
    lua_pushnumber(L, img->sizeX);
    return 1;
}

static int image_height(lua_State *L){
    UL_IMAGE * img = lua_touserdata(L, 1);
    assert(L, img != NULL, "Bad image ressource");
    lua_pushnumber(L, img->sizeY);
    return 1;
}

static int image_scale(lua_State *L){
    UL_IMAGE * img = lua_touserdata(L, 1);
    int width = (int)luaL_checknumber(L, 2);
    int height = (int)luaL_checknumber(L, 3);
    assert(L, img != NULL, "Bad image ressource");
    assert(L, width >= 0, "Width must be positive");
    assert(L, height >= 0, "Height must be positive");
    img->stretchX = width;
    img->stretchY = height;
    return 0;
}

static int image_rotate(lua_State *L){
    UL_IMAGE * img = lua_touserdata(L, 1);
    int angle = (int)luaL_checknumber(L, 2);
    int x=0, y=0;
    if(lua_isnumber(L, 3)) x = (int)luaL_checknumber(L, 3);
    if(lua_isnumber(L, 4)) y = (int)luaL_checknumber(L, 4);
    assert(L, img != NULL, "Bad image ressource");
    assert(L, angle >= 0 && angle <= 511, "Angle must be between 0 and 511");
    img->centerX = x;
    img->centerY = y;
    img->angle = (32700 * angle) / 511;
    return 0;
}

static int image_rotateDegree(lua_State *L){
    UL_IMAGE * img = lua_touserdata(L, 1);
    int angle = (int)luaL_checknumber(L, 2);
    int x=0, y=0;
    if(lua_isnumber(L, 3)) x = (int)luaL_checknumber(L, 3);
    if(lua_isnumber(L, 4)) y = (int)luaL_checknumber(L, 4);
    assert(L, img != NULL, "Bad image ressource");
    assert(L, angle >= 0 && angle <= 360, "Angle must be between 0 and 360");
    img->centerX = x;
    img->centerY = y;
    img->angle = (32700 * angle) / 360;
    return 0;
}

static int image_mirrorH(lua_State *L){
    UL_IMAGE * img = lua_touserdata(L, 1);
    assert(L, img != NULL, "Bad image ressource");
    bool yes = (bool)lua_toboolean(L, 2);
    ulMirrorImageH(img, yes);
    return 0;
}

static int image_mirrorV(lua_State *L){
    UL_IMAGE * img = lua_touserdata(L, 1);
    assert(L, img != NULL, "Bad image ressource");
    bool yes = (bool)lua_toboolean(L, 2);
    ulMirrorImageV(img, yes);
    return 0;
}

static int image_setTint(lua_State *L){
    UL_IMAGE * img = lua_touserdata(L, 1);
    assert(L, img != NULL, "Bad image ressource");
    int color = (int)luaL_checknumber(L, 2);
    ulSetImageTint(img, color);
    return 0;
}

static const luaL_Reg imagelib[] = {
    {"load", image_load},
    {"destroy", image_destroy},
    {"width", image_width},
    {"height", image_height},
    {"scale", image_scale},
    {"rotate", image_rotate},
    {"rotateDegree", image_rotateDegree},
    {"mirrorH", image_mirrorH},
    {"mirrorV", image_mirrorV},
    {"setTint", image_setTint},
    {NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_image (lua_State *L) {
    luaL_register(L, LUA_IMAGELIBNAME, imagelib);
    return 1;
}
