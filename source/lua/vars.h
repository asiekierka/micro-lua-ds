#ifndef LUA_DS_VARS_H_
#define LUA_DS_VARS_H_

#include <ulib/ulib.h>
#include "ds_lib.h"

#ifdef assert
#undef assert
#endif
#define assert(l, cond, text) if(!(cond)){ luaL_error(l, text); return 0;}

#define LUA_RAM  0
#define LUA_VRAM  1

#define CANVAS_TYPE_LINE 0
#define CANVAS_TYPE_POINT 1
#define CANVAS_TYPE_RECT 2
#define CANVAS_TYPE_FILLRECT 3
#define CANVAS_TYPE_GRADIENTRECT 4
#define CANVAS_TYPE_TEXT 5
#define CANVAS_TYPE_TEXTFONT 6
#define CANVAS_TYPE_TEXTBOX 7
#define CANVAS_TYPE_IMAGE 8

#define ATTR_X1 0
#define ATTR_Y1 1
#define ATTR_X2 2
#define ATTR_Y2 3
#define ATTR_X3 4
#define ATTR_Y3 5
#define ATTR_COLOR 6
#define ATTR_COLOR1 7
#define ATTR_COLOR2 8
#define ATTR_COLOR3 9
#define ATTR_COLOR4 10
#define ATTR_TEXT 11
#define ATTR_VISIBLE 12
#define ATTR_FONT 13
#define ATTR_IMAGE 14

#define MAX_OBJECTS 20000

extern int SCREEN_UP_DISPLAY;
extern int SCREEN_DOWN_DISPLAY;
extern int spaceBetweenScreens;

extern int alphaGroup;

extern u8 Wifi_init;

typedef struct mapObj{
    UL_IMAGE * image;
    unsigned short * table;
    int width, height;
    int tileWidth, tileHeight;
    int scrollX, scrollY;
    int spaceX, spaceY;
} Map;

#endif
