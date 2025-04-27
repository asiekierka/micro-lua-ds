/*
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ds_screen_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <ulib/ulib.h>

#include "vars.h"

int SCREEN_UP_DISPLAY;
int SCREEN_DOWN_DISPLAY;
int spaceBetweenScreens;

int alphaGroup = 1;
int alphaLevel = 100;

static int screen_init(lua_State *L) {
    SCREEN_UP_DISPLAY = 0;
    SCREEN_DOWN_DISPLAY = 1;
    spaceBetweenScreens = 0;
    return 0;
}

static int screen_switch(lua_State *L) {
    int tmp = SCREEN_UP_DISPLAY;
    SCREEN_UP_DISPLAY = SCREEN_DOWN_DISPLAY;
    SCREEN_DOWN_DISPLAY = tmp;
    return 0;
}

static int screen_blit(lua_State *L) {
    int screen = (int)luaL_checknumber(L, 1);
    int x, y;
    if(lua_isnumber(L, 2)) x = (int)luaL_checknumber(L, 2); else{ luaL_error(L, "x must be a number"); return 0;}
    if(lua_isnumber(L, 3)) y = (int)luaL_checknumber(L, 3); else{ luaL_error(L, "y must be a number"); return 0;}
    UL_IMAGE * img = lua_touserdata(L, 4);
    int sourcex=-1, sourcey=-1, width=-1, height=-1;
    if(lua_isnumber(L, 5)) sourcex = (int)luaL_checknumber(L, 5);
    if(lua_isnumber(L, 6)) sourcey = (int)luaL_checknumber(L, 6);
    if(lua_isnumber(L, 7)) width  = (int)luaL_checknumber(L, 7);
    if(lua_isnumber(L, 8)) height = (int)luaL_checknumber(L, 8);
    assert(L, screen == SCREEN_UP_DISPLAY || screen == SCREEN_DOWN_DISPLAY, "Bad screen number");
    assert(L, img != NULL, "Bad image ressource");
    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
        if(height != -1) ulSetImageTile(img, sourcex, sourcey, sourcex+width, sourcey+height);
        else{
            if(sourcey != -1) ulSetImageTile(img, sourcex, sourcey, img->sizeX, img->sizeY);
        }
        ulDrawImageXY(img, x+img->centerX, y+img->centerY);
        //ulResetImageTile(img);
    }
    return 0;
}

static int screen_getMainLcd(lua_State *L) {
    lua_pushboolean(L, ulGetMainLcd());
    return 1;
}

static int screen_startDrawing2D(lua_State *L) {
    ulStartDrawing2D();
    return 0;
}

static int screen_endDrawing(lua_State *L) {
    ulEndDrawing();
    return 0;
}

static int screen_waitForVBL(lua_State *L) {
    ulSyncFrame();
    return 0;
}

static int screen_getLayer(lua_State *L) {
    lua_pushnumber(L, alphaGroup);
    return 1;
}

static int screen_getAlphaLevel(lua_State *L) {
    lua_pushnumber(L, alphaLevel);
    return 1;
}

static int screen_setAlpha(lua_State *L) {
    int level = (int)luaL_checknumber(L, 1);
    if (level == 100) {
    	ulSetAlpha(UL_FX_DEFAULT, 0, 0);
        alphaGroup = 1;
        alphaLevel = 100;
    } else {
        int group = 0;
        if (lua_isnumber(L, 2)) group = (int)luaL_checknumber(L, 2);
        if (group < 1) group = alphaGroup;
        level = floor(level * 31 / 99);         // level must actually be between 1 and 31 (0 = wireframe)
        if (level == 0) level = 1;
        ulSetAlpha(UL_FX_ALPHA, level, group);
        if (alphaLevel != level) {
            alphaGroup = group + 1;
            alphaLevel = level;
        }
    }
    return 0;
}

static int screen_print(lua_State *L) {
    int screen = (int)luaL_checknumber(L, 1);
    int x = (int)luaL_checknumber(L, 2);
    int y = (int)luaL_checknumber(L, 3);
    char * text = (char *)luaL_checkstring(L, 4);
    int color = RGB15(31, 31, 31);
    if(lua_isnumber(L, 5)) color = (int)luaL_checknumber(L, 5);
    assert(L, screen == SCREEN_UP_DISPLAY || screen == SCREEN_DOWN_DISPLAY, "Bad screen number");
    /*assert(L, x>=0, "x must be >= 0");
    assert(L, y>=0, "y must be >= 0");*/
    assert(L, text != NULL , "Text can't be null");
    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
        ulSetTextColor(color);
        ulDrawString(x, y, text);
    }
    return 0;
}

static int screen_printFont(lua_State *L) {
    int screen = (int)luaL_checknumber(L, 1);
    int x = (int)luaL_checknumber(L, 2);
    int y = (int)luaL_checknumber(L, 3);
    char * text = (char *)luaL_checkstring(L, 4);
    int color = RGB15(31, 31, 31);
    if(lua_isnumber(L, 5)) color = (int)luaL_checknumber(L, 5);
    UL_FONT * font = lua_touserdata(L, 6);
    assert(L, screen == SCREEN_UP_DISPLAY || screen == SCREEN_DOWN_DISPLAY, "Bad screen number");
    /*assert(L, x>=0, "x must be >= 0");
    assert(L, y>=0, "y must be >= 0");*/
    assert(L, text != NULL, "Text can't be null");
    assert(L, font != NULL, "Font can't be null");
    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
        ulSetFont(font);
        ulSetTextColor(color);
        ulDrawString(x, y, text);
        ulSetFont(ul_lcdFont);
    }
    return 0;
}

static int screen_drawLine(lua_State *L) {
    int screen = (int)luaL_checknumber(L, 1);
    int x0 = (int)luaL_checknumber(L, 2);
    int y0 = (int)luaL_checknumber(L, 3);
    int x1 = (int)luaL_checknumber(L, 4);
    int y1 = (int)luaL_checknumber(L, 5);
    int color = (int)luaL_checknumber(L, 6);
    /*assert(L, x0>=0, "x0 must be >= 0");
    assert(L, y0>=0, "y0 must be >= 0");
    assert(L, x1>=0, "x1 must be >= 0");
    assert(L, y1>=0, "y1 must be >= 0");*/
    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
        ulDrawLine(x0, y0, x1, y1, color);
    }
    return 0;
}

static int screen_drawPoint(lua_State *L) {
    int screen = (int)luaL_checknumber(L, 1);
    int x = (int)luaL_checknumber(L, 2);
    int y = (int)luaL_checknumber(L, 3);
    int color = (int)luaL_checknumber(L, 4);
    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd()))
        ulDrawLine(x, y, x, y, color);
    return 0;
}

static int screen_drawRect(lua_State *L) {
    int screen = (int)luaL_checknumber(L, 1);
    int x0 = (int)luaL_checknumber(L, 2);
    int y0 = (int)luaL_checknumber(L, 3);
    int x1 = (int)luaL_checknumber(L, 4);
    int y1 = (int)luaL_checknumber(L, 5);
    int color = (int)luaL_checknumber(L, 6);
    /*assert(L, x0>=0, "x0 must be >= 0");
    assert(L, y0>=0, "y0 must be >= 0");
    assert(L, x1>=0, "x1 must be >= 0");
    assert(L, y1>=0, "y1 must be >= 0");*/
    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
        ulDrawRect(x0, y0, x1, y1, color);
    }
    return 0;
}

static int screen_drawFillRect(lua_State *L) {
    int screen = (int)luaL_checknumber(L, 1);
    int x0 = (int)luaL_checknumber(L, 2);
    int y0 = (int)luaL_checknumber(L, 3);
    int x1 = (int)luaL_checknumber(L, 4);
    int y1 = (int)luaL_checknumber(L, 5);
    int color = (int)luaL_checknumber(L, 6);
    /*assert(L, x0>=0, "x0 must be >= 0");
    assert(L, y0>=0, "y0 must be >= 0");
    assert(L, x1>=0, "x1 must be >= 0");
    assert(L, y1>=0, "y1 must be >= 0");*/
    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
        ulDrawFillRect(x0, y0, x1, y1, color);
    }
    return 0;
}

static int screen_drawGradientRect(lua_State *L) {
    int screen = (int)luaL_checknumber(L, 1);
    int x0 = (int)luaL_checknumber(L, 2);
    int y0 = (int)luaL_checknumber(L, 3);
    int x1 = (int)luaL_checknumber(L, 4);
    int y1 = (int)luaL_checknumber(L, 5);
    int color1 = (int)luaL_checknumber(L, 6);
    int color2 = (int)luaL_checknumber(L, 7);
    int color3 = (int)luaL_checknumber(L, 8);
    int color4 = (int)luaL_checknumber(L, 9);
    /*assert(L, x0>=0, "x0 must be >= 0");
    assert(L, y0>=0, "y0 must be >= 0");
    assert(L, x1>=0, "x1 must be >= 0");
    assert(L, y1>=0, "y1 must be >= 0");*/
    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
        ulDrawGradientRect(x0, y0, x1, y1, color1, color2, color3, color4);
    }
    return 0;
}

static int screen_drawTextBox(lua_State *L) {
    int screen = (int)luaL_checknumber(L, 1);
    int x0 = (int)luaL_checknumber(L, 2);
    int y0 = (int)luaL_checknumber(L, 3);
    int x1 = (int)luaL_checknumber(L, 4);
    int y1 = (int)luaL_checknumber(L, 5);
    char * text = (char *)luaL_checkstring(L, 6);
    int color = RGB15(31, 31, 31);
    if(lua_isnumber(L, 7)) color = (int)luaL_checknumber(L, 7);
    /*assert(L, x0>=0, "x0 must be >= 0");
    assert(L, y0>=0, "y0 must be >= 0");
    assert(L, x1>=0, "x1 must be >= 0");
    assert(L, y1>=0, "y1 must be >= 0");*/
    ulSetTextColor(color);
    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
        ulDrawTextBox(x0, y0, x1, y1, text, UL_PF_PAL8);
    }
    return 0;
}

static int screen_setSpaceBetweenScreens(lua_State *L) {
    spaceBetweenScreens = (int)luaL_checknumber(L, 1);
    return 0;
}

static int screen_drawTexturedQuad(lua_State *L){
	int screen = (int)luaL_checknumber(L, 1);
    int x0 = (int)luaL_checknumber(L, 2);
    int y0 = (int)luaL_checknumber(L, 3);
    int x1 = (int)luaL_checknumber(L, 4);
    int y1 = (int)luaL_checknumber(L, 5);
    int x2 = (int)luaL_checknumber(L, 6);
    int y2 = (int)luaL_checknumber(L, 7);
    int x3 = (int)luaL_checknumber(L, 8);
    int y3 = (int)luaL_checknumber(L, 9);
	UL_IMAGE * img = lua_touserdata(L, 10);
    int sourcex=-1, sourcey=-1, width=-1, height=-1;
    if(lua_isnumber(L, 11)) sourcex = (int)luaL_checknumber(L, 11);
    if(lua_isnumber(L, 12)) sourcey = (int)luaL_checknumber(L, 12);
    if(lua_isnumber(L, 13)) width  = (int)luaL_checknumber(L, 13);
    if(lua_isnumber(L, 14)) height = (int)luaL_checknumber(L, 14);
    
    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
		if(height != -1) ulSetImageTile(img, sourcex, sourcey, sourcex+width, sourcey+height);
		else{
			if(sourcey != -1) ulSetImageTile(img, sourcex, sourcey, img->sizeX, img->sizeY);
		}
		
		ulSetTexture(img);

		//Begins drawing quadliterals
		ulVertexBegin(GL_QUADS);
		
		//Drawing order for a quad is: top-left, bottom-left, bottom-right, top-right.
		ulVertexUVXY(img->offsetX0, img->offsetY0, x0, y0);
		ulVertexUVXY(img->offsetX0, img->offsetY1, x1, y1);
		ulVertexUVXY(img->offsetX1, img->offsetY1, x2, y2);
		ulVertexUVXY(img->offsetX1, img->offsetY0, x3, y3);

		//End there. You could also continue and draw other quads (issue 4 new vertex commands)
		ulVertexEnd();

		//Auto-increment depth
		ulVertexHandleDepth();
		
		ulResetImageTile(img);
	}
	return 0;
}

static int screen_drawTexturedTriangle(lua_State *L){
	int screen = (int)luaL_checknumber(L, 1);
    int x0 = (int)luaL_checknumber(L, 2);
    int y0 = (int)luaL_checknumber(L, 3);
    int x1 = (int)luaL_checknumber(L, 4);
    int y1 = (int)luaL_checknumber(L, 5);
    int x2 = (int)luaL_checknumber(L, 6);
    int y2 = (int)luaL_checknumber(L, 7);
	UL_IMAGE * img = lua_touserdata(L, 8);
    int sourcex=-1, sourcey=-1, width=-1, height=-1;
    if(lua_isnumber(L, 9)) sourcex = (int)luaL_checknumber(L, 9);
    if(lua_isnumber(L, 10)) sourcey = (int)luaL_checknumber(L, 10);
    if(lua_isnumber(L, 11)) width  = (int)luaL_checknumber(L, 11);
    if(lua_isnumber(L, 12)) height = (int)luaL_checknumber(L, 12);
    
    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
		if(height != -1) ulSetImageTile(img, sourcex, sourcey, sourcex+width, sourcey+height);
		else{
			if(sourcey != -1) ulSetImageTile(img, sourcex, sourcey, img->sizeX, img->sizeY);
		}
		
		ulSetTexture(img);

		//Begins drawing quadliterals
		ulVertexBegin(GL_TRIANGLES);
		
		//Drawing order for a quad is: top-left, bottom-left, bottom-right, top-right.
		ulVertexUVXY(img->offsetX0, img->offsetY0, x0, y0);
		ulVertexUVXY(img->offsetX0, img->offsetY1, x1, y1);
		ulVertexUVXY(img->offsetX1, img->offsetY1, x2, y2);

		//End there. You could also continue and draw other quads (issue 4 new vertex commands)
		ulVertexEnd();

		//Auto-increment depth
		ulVertexHandleDepth();
		
		ulResetImageTile(img);
	}
	return 0;
}

static const luaL_Reg screenlib[] = {
    {"init", screen_init},
    {"switch", screen_switch},
    {"getMainLcd", screen_getMainLcd},
    {"startDrawing2D", screen_startDrawing2D},
    {"endDrawing", screen_endDrawing},
    {"waitForVBL", screen_waitForVBL},
    {"getLayer", screen_getLayer},
    {"getAlphaLevel", screen_getAlphaLevel},
    {"setAlpha", screen_setAlpha},
    {"print", screen_print},
    {"printFont", screen_printFont},
    {"blit", screen_blit},
    {"drawLine", screen_drawLine},
    {"drawPoint", screen_drawPoint},
    {"drawRect", screen_drawRect},
    {"drawFillRect", screen_drawFillRect},
    {"drawGradientRect", screen_drawGradientRect},
    {"drawTextBox", screen_drawTextBox},
    {"setSpaceBetweenScreens", screen_setSpaceBetweenScreens},
    {"drawTexturedQuad", screen_drawTexturedQuad},
    {"drawTexturedTriangle", screen_drawTexturedTriangle},
    {NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_screen (lua_State *L) {
    luaL_register(L, LUA_SCREENLIBNAME, screenlib);
    return 1;
}
