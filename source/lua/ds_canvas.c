/*
*
*/

#include <stdlib.h>
#include <stdio.h>

#define ds_canvas_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <ulib/ulib.h>

#include "vars.h"

typedef struct CanvasObjectObj{
    u8 type;
    int x1, y1, x2, y2, x3, y3;
    int color, color1, color2, color3, color4;
    char * text;
    //char text[255];
    bool visible;
    UL_FONT * font;
    UL_IMAGE * image;
}CanvasObject;

typedef struct CanvasObj{
    int nb;
    CanvasObject * list[MAX_OBJECTS];
}Canvas;

static int canvas_new(lua_State *L){
    Canvas * canvas;
    canvas = malloc(sizeof(Canvas));
    canvas->nb = 0;
    lua_pushlightuserdata(L, canvas);
    return 1;
}

static int canvas_destroy(lua_State *L){
    Canvas * canvas = lua_touserdata(L, 1);
    int i;
    for(i=0;i<canvas->nb;i++){
    	free(canvas->list[i]);
    }
    free(canvas);
    return 0;
}

static int canvas_newLine(lua_State *L){
    int x1 = (int)luaL_checknumber(L, 1);
    int y1 = (int)luaL_checknumber(L, 2);
    int x2 = (int)luaL_checknumber(L, 3);
    int y2 = (int)luaL_checknumber(L, 4);
    int color = (int)luaL_checknumber(L, 5);
    assert(L, x1>=0, "x1 must be >= 0");
    assert(L, y1>=0, "y1 must be >= 0");
    assert(L, x2>=0, "x2 must be >= 0");
    assert(L, y2>=0, "y2 must be >= 0");
    CanvasObject * co;
    co = malloc(sizeof(CanvasObject));
    co->visible = true;
    co->type = CANVAS_TYPE_LINE;
    co->x1 = x1;
    co->y1 = y1;
    co->x2 = x2;
    co->y2 = y2;
    co->color = color;
    lua_pushlightuserdata(L, co);
    return 1;
}

static int canvas_newImage(lua_State *L){
    CanvasObject * co;
    co = malloc(sizeof(CanvasObject));
    if(lua_isnumber(L, 1)) co->x1 = (int)luaL_checknumber(L, 1); else{ luaL_error(L, "x1 must be a number"); return 0;}
    if(lua_isnumber(L, 1)) co->y1 = (int)luaL_checknumber(L, 2); else{ luaL_error(L, "y1 must be a number"); return 0;}
    UL_IMAGE * img = lua_touserdata(L, 3);
    co->x2 = -1; co->x3 = -1; co->y2 = -1; co->y3 = -1;
    if(lua_isnumber(L, 4)) co->x2 = (int)luaL_checknumber(L, 4);
    if(lua_isnumber(L, 5)) co->y2 = (int)luaL_checknumber(L, 5);
    if(lua_isnumber(L, 6)) co->x3 = (int)luaL_checknumber(L, 6);
    if(lua_isnumber(L, 7)) co->y3 = (int)luaL_checknumber(L, 7);
    co->visible = true;
    co->type = CANVAS_TYPE_IMAGE;
    co->image = img;
    lua_pushlightuserdata(L, co);
    return 1;
}

static int canvas_newText(lua_State *L){
    int x1 = (int)luaL_checknumber(L, 1);
    int y1 = (int)luaL_checknumber(L, 2);
    char * text = (char *)luaL_checkstring(L, 3);
    int color = RGB15(31, 31, 31);
    if(lua_isnumber(L, 4)) color = (int)luaL_checknumber(L, 4);
    assert(L, x1>=0, "x1 must be >= 0");
    assert(L, y1>=0, "y1 must be >= 0");
    assert(L, text != NULL , "Text can't be null");
    CanvasObject * co;
    co = malloc(sizeof(CanvasObject));
    co->visible = true;
    co->type = CANVAS_TYPE_TEXT;
    co->x1 = x1;
    co->y1 = y1;
    co->text = strdup(text);
    co->color = color;
    lua_pushlightuserdata(L, co);
    return 1;
}

static int canvas_newTextBox(lua_State *L){
    int x1 = (int)luaL_checknumber(L, 1);
    int y1 = (int)luaL_checknumber(L, 2);
    int x2 = (int)luaL_checknumber(L, 3);
    int y2 = (int)luaL_checknumber(L, 4);
    char * text = (char *)luaL_checkstring(L, 5);
    int color = RGB15(31, 31, 31);
    if(lua_isnumber(L, 4)) color = (int)luaL_checknumber(L, 6);
    assert(L, x1>=0, "x1 must be >= 0");
    assert(L, y1>=0, "y1 must be >= 0");
    assert(L, x2>=0, "x2 must be >= 0");
    assert(L, y2>=0, "y2 must be >= 0");
    assert(L, text != NULL , "Text can't be null");
    CanvasObject * co;
    co = malloc(sizeof(CanvasObject));
    co->visible = true;
    co->type = CANVAS_TYPE_TEXTBOX;
    co->x1 = x1;
    co->y1 = y1;
    co->x2 = x2;
    co->y2 = y2;
    co->text = strdup(text);
    co->color = color;
    lua_pushlightuserdata(L, co);
    return 1;
}

static int canvas_newTextFont(lua_State *L){
    int x1 = (int)luaL_checknumber(L, 1);
    int y1 = (int)luaL_checknumber(L, 2);
    char * text = (char *)luaL_checkstring(L, 3);
    int color = RGB15(31, 31, 31);
    if(lua_isnumber(L, 5)) color = (int)luaL_checknumber(L, 4);
    UL_FONT * font = lua_touserdata(L, 5);
    assert(L, x1>=0, "x1 must be >= 0");
    assert(L, y1>=0, "y1 must be >= 0");
    assert(L, text != NULL, "Text can't be null");
    assert(L, font != NULL, "Font can't be null");
    CanvasObject * co;
    co = malloc(sizeof(CanvasObject));
    co->visible = true;
    co->type = CANVAS_TYPE_TEXTFONT;
    co->x1 = x1;
    co->y1 = y1;
    co->text = strdup(text);
    co->color = color;
    co->font = font;
    lua_pushlightuserdata(L, co);
    return 1;
}

static int canvas_newPoint(lua_State *L){
    int x1 = (int)luaL_checknumber(L, 1);
    int y1 = (int)luaL_checknumber(L, 2);
    int color = (int)luaL_checknumber(L, 3);
    assert(L, x1>=0, "x1 must be >= 0");
    assert(L, y1>=0, "y1 must be >= 0");
    CanvasObject * co;
    co = malloc(sizeof(CanvasObject));
    co->visible = true;
    co->type = CANVAS_TYPE_POINT;
    co->x1 = x1;
    co->y1 = y1;
    co->color = color;
    lua_pushlightuserdata(L, co);
    return 1;
}

static int canvas_newRect(lua_State *L){
    int x1 = (int)luaL_checknumber(L, 1);
    int y1 = (int)luaL_checknumber(L, 2);
    int x2 = (int)luaL_checknumber(L, 3);
    int y2 = (int)luaL_checknumber(L, 4);
    int color = (int)luaL_checknumber(L, 5);
    assert(L, x1>=0, "x1 must be >= 0");
    assert(L, y1>=0, "y1 must be >= 0");
    assert(L, x2>=0, "x2 must be >= 0");
    assert(L, y2>=0, "y2 must be >= 0");
    CanvasObject * co;
    co = malloc(sizeof(CanvasObject));
    co->visible = true;
    co->type = CANVAS_TYPE_RECT;
    co->x1 = x1;
    co->y1 = y1;
    co->x2 = x2;
    co->y2 = y2;
    co->color = color;
    lua_pushlightuserdata(L, co);
    return 1;
}

static int canvas_newFillRect(lua_State *L){
    int x1 = (int)luaL_checknumber(L, 1);
    int y1 = (int)luaL_checknumber(L, 2);
    int x2 = (int)luaL_checknumber(L, 3);
    int y2 = (int)luaL_checknumber(L, 4);
    int color = (int)luaL_checknumber(L, 5);
    assert(L, x1>=0, "x1 must be >= 0");
    assert(L, y1>=0, "y1 must be >= 0");
    assert(L, x2>=0, "x2 must be >= 0");
    assert(L, y2>=0, "y2 must be >= 0");
    CanvasObject * co;
    co = malloc(sizeof(CanvasObject));
    co->visible = true;
    co->type = CANVAS_TYPE_FILLRECT;
    co->x1 = x1;
    co->y1 = y1;
    co->x2 = x2;
    co->y2 = y2;
    co->color = color;
    lua_pushlightuserdata(L, co);
    return 1;
}

static int canvas_newGradientRect(lua_State *L){
    int x1 = (int)luaL_checknumber(L, 1);
    int y1 = (int)luaL_checknumber(L, 2);
    int x2 = (int)luaL_checknumber(L, 3);
    int y2 = (int)luaL_checknumber(L, 4);
    int color1 = (int)luaL_checknumber(L, 5);
    int color2 = (int)luaL_checknumber(L, 6);
    int color3 = (int)luaL_checknumber(L, 7);
    int color4 = (int)luaL_checknumber(L, 8);
    assert(L, x1>=0, "x1 must be >= 0");
    assert(L, y1>=0, "y1 must be >= 0");
    assert(L, x2>=0, "x2 must be >= 0");
    assert(L, y2>=0, "y2 must be >= 0");
    CanvasObject * co;
    co = malloc(sizeof(CanvasObject));
    co->visible = true;
    co->type = CANVAS_TYPE_GRADIENTRECT;
    co->x1 = x1;
    co->y1 = y1;
    co->x2 = x2;
    co->y2 = y2;
    co->color1 = color1;
    co->color2 = color2;
    co->color3 = color3;
    co->color4 = color4;
    lua_pushlightuserdata(L, co);
    return 1;
}

static int canvas_add(lua_State *L){
    Canvas * canvas = lua_touserdata(L, 1);
    CanvasObject * co = lua_touserdata(L, 2);
    canvas->list[canvas->nb] = co;
    canvas->nb++;
    return 0;
}

static int canvas_setAttr(lua_State *L){
    CanvasObject * co = lua_touserdata(L, 1);
    switch((int)luaL_checknumber(L, 2)){
        case ATTR_X1:
            co->x1 = (int)luaL_checknumber(L, 3);
            break;
        case ATTR_Y1:
            co->y1 = (int)luaL_checknumber(L, 3);
            break;
        case ATTR_X2:
            co->x2 = (int)luaL_checknumber(L, 3);
            break;
        case ATTR_Y2:
            co->y2 = (int)luaL_checknumber(L, 3);
            break;
        case ATTR_X3:
            co->x3 = (int)luaL_checknumber(L, 3);
            break;
        case ATTR_Y3:
            co->y3 = (int)luaL_checknumber(L, 3);
            break;
        case ATTR_COLOR:
            co->color = (int)luaL_checknumber(L, 3);
            break;
        case ATTR_COLOR1:
            co->color1 = (int)luaL_checknumber(L, 3);
            break;
        case ATTR_COLOR2:
            co->color2 = (int)luaL_checknumber(L, 3);
            break;
        case ATTR_COLOR3:
            co->color3 = (int)luaL_checknumber(L, 3);
            break;
        case ATTR_COLOR4:
            co->color4 = (int)luaL_checknumber(L, 3);
            break;
        case ATTR_TEXT:
            free(co->text);
            co->text = strdup((char *)luaL_checkstring(L, 3));
            break;
        case ATTR_VISIBLE:
            co->visible = (bool)lua_toboolean(L, 3);
            break;
        case ATTR_FONT:
            co->font = lua_touserdata(L, 3);
            break;
        case ATTR_IMAGE:
            co->image = lua_touserdata(L, 3);
            break;
        default:
            luaL_error(L, "Unknown attribute");
            return 0;
            break;
    }
    return 0;
}


static int canvas_getAttr(lua_State *L){
    CanvasObject * co = lua_touserdata(L, 1);
    switch((int)luaL_checknumber(L, 2)){
        case ATTR_X1:
            lua_pushnumber(L, co->x1);
            break;
        case ATTR_Y1:
            lua_pushnumber(L, co->y1);
            break;
        case ATTR_X2:
            lua_pushnumber(L, co->x2);
            break;
        case ATTR_Y2:
            lua_pushnumber(L, co->y2);
            break;
        case ATTR_X3:
            lua_pushnumber(L, co->x3);
            break;
        case ATTR_Y3:
            lua_pushnumber(L, co->y3);
            break;
        case ATTR_COLOR:
            lua_pushnumber(L, co->color);
            break;
        case ATTR_COLOR1:
            lua_pushnumber(L, co->color1);
            break;
        case ATTR_COLOR2:
            lua_pushnumber(L, co->color2);
            break;
        case ATTR_COLOR3:
            lua_pushnumber(L, co->color3);
            break;
        case ATTR_COLOR4:
            lua_pushnumber(L, co->color4);
            break;
        case ATTR_TEXT:
            lua_pushstring(L, co->text);
            break;
        case ATTR_VISIBLE:
            lua_pushboolean(L, co->visible);
            break;
        case ATTR_FONT:
            lua_pushlightuserdata(L, co->font);
            break;
        case ATTR_IMAGE:
            lua_pushlightuserdata(L, co->image);
            break;
        default:
            luaL_error(L, "Unknown attribute");
            return 0;
            break;
    }
    return 1;
}

static int canvas_draw(lua_State *L){
    int screen = (int)luaL_checknumber(L, 1);
    Canvas * canvas = lua_touserdata(L, 2);
    int x = (int)luaL_checknumber(L, 3);
    int y = (int)luaL_checknumber(L, 4);
    assert(L, screen == SCREEN_UP_DISPLAY || screen == SCREEN_DOWN_DISPLAY, "Bad screen number");
    int i;
    for(i=0; i<canvas->nb;i++){
        if(canvas->list[i]->visible){
            switch(canvas->list[i]->type){
                // Draw Lines
                case CANVAS_TYPE_LINE:
                    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
                        ulDrawLine(canvas->list[i]->x1+x, canvas->list[i]->y1+y, canvas->list[i]->x2+x, canvas->list[i]->y2+y, canvas->list[i]->color);
                    }
                    break;
                // Draw Points
                case CANVAS_TYPE_POINT:
                    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
                        ulDrawLine(canvas->list[i]->x1+x, canvas->list[i]->y1+y, canvas->list[i]->x1+x, canvas->list[i]->y1+y, canvas->list[i]->color);
                    }
                    break;
                // Draw Rect
                case CANVAS_TYPE_RECT:
                    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
                        ulDrawRect(canvas->list[i]->x1+x, canvas->list[i]->y1+y, canvas->list[i]->x2+x, canvas->list[i]->y2+y, canvas->list[i]->color);
                    }
                    break;
                // Draw FillRect
                case CANVAS_TYPE_FILLRECT:
                    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
                        ulDrawFillRect(canvas->list[i]->x1+x, canvas->list[i]->y1+y, canvas->list[i]->x2+x, canvas->list[i]->y2+y, canvas->list[i]->color);
                    }
                    break;
                // Draw GradientRect
                case CANVAS_TYPE_GRADIENTRECT:
                    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
                        ulDrawGradientRect(canvas->list[i]->x1+x, canvas->list[i]->y1+y, canvas->list[i]->x2+x, canvas->list[i]->y2+y, canvas->list[i]->color1, canvas->list[i]->color2, canvas->list[i]->color3, canvas->list[i]->color4);
                    }
                    break;
                // Draw Text
                case CANVAS_TYPE_TEXT:
                    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
                        ulSetTextColor(canvas->list[i]->color);
                        ulDrawString(canvas->list[i]->x1+x, canvas->list[i]->y1+y, canvas->list[i]->text);
                    }
                    break;
                // Draw TextFont
                case CANVAS_TYPE_TEXTFONT:
                    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
                        ulSetFont(canvas->list[i]->font);
                        ulSetTextColor(canvas->list[i]->color);
                        ulDrawString(canvas->list[i]->x1+x, canvas->list[i]->y1+y, canvas->list[i]->text);
                        ulSetFont(ul_lcdFont);
                    }
                    break;
                // Draw TextBox
                case CANVAS_TYPE_TEXTBOX:
                    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
                        ulSetTextColor(canvas->list[i]->color);
                        ulDrawTextBox(canvas->list[i]->x1+x, canvas->list[i]->y1+y, canvas->list[i]->x2+x, canvas->list[i]->y2+y, canvas->list[i]->text, UL_PF_PAL8);
                    }
                    break;
                // Draw Image
                case CANVAS_TYPE_IMAGE:
                    if(canvas->list[i]->y3 != -1) ulSetImageTile(canvas->list[i]->image, canvas->list[i]->x2, canvas->list[i]->y2, canvas->list[i]->x2+canvas->list[i]->x3, canvas->list[i]->y2+canvas->list[i]->y3);
                    else{
                        if(canvas->list[i]->y2 != -1) ulSetImageTile(canvas->list[i]->image, canvas->list[i]->x2, canvas->list[i]->y2, canvas->list[i]->image->sizeX, canvas->list[i]->image->sizeY);
                    }
                    if ((screen == SCREEN_UP_DISPLAY && ulGetMainLcd()) || (screen == SCREEN_DOWN_DISPLAY && !ulGetMainLcd())){
                        ulDrawImageXY(canvas->list[i]->image, canvas->list[i]->x1+x+canvas->list[i]->image->centerX, canvas->list[i]->y1+y+canvas->list[i]->image->centerY);
                        // ulResetImageTile(canvas->list[i]->image);
                    }
                    break;
                // Default
                default:
                    break;
            }
        }
    }
    return 0;
}

static int canvas_setObjOnTop(lua_State *L){
    Canvas * canvas = lua_touserdata(L, 1);
    CanvasObject * co = lua_touserdata(L, 2);
	// search the object
	int i;
	int pos = -1;
	CanvasObject * buff;
	for(i=0; i<canvas->nb; i++){
		if(co == canvas->list[i]){
			pos = i;
			break;
		}
	}
	// Si trouve, on le copie, on remonte tout de 1 a partir de sa position et on le colle en fin
	if(pos>-1){
		buff = canvas->list[pos];
		for(i=pos; i<(canvas->nb -1); i++){
			canvas->list[i] = canvas->list[i+1];
		}
		canvas->list[canvas->nb-1] = buff;
	}
	return 0;
}

static int canvas_removeObj(lua_State *L){
    Canvas * canvas = lua_touserdata(L, 1);
    CanvasObject * co = lua_touserdata(L, 2);
    // search the object
	int i;
	int pos = -1;
	CanvasObject * buff;
	for(i=0; i<canvas->nb; i++){
		if(co == canvas->list[i]){
			pos = i;
			break;
		}
	}
	// Si trouve, on le supprime, on remonte tout de 1 a partir de sa position
	if(pos>-1){
		buff = canvas->list[pos];
		for(i=pos; i<(canvas->nb -1); i++){
			canvas->list[i] = canvas->list[i+1];
		}
		// et on le supprime
		free(buff);
		canvas->nb--;
	}
    return 0;
}

static const luaL_Reg canvaslib[] = {
    {"new", canvas_new},
    {"destroy", canvas_destroy},
    {"newLine", canvas_newLine},
    {"newPoint", canvas_newPoint},
    {"newRect", canvas_newRect},
    {"newFillRect", canvas_newFillRect},
    {"newGradientRect", canvas_newGradientRect},
    {"newText", canvas_newText},
    {"newTextFont", canvas_newTextFont},
    {"newTextBox", canvas_newTextBox},
    {"newImage", canvas_newImage},
    {"add", canvas_add},
    {"draw", canvas_draw},
    {"setAttr", canvas_setAttr},
    {"getAttr", canvas_getAttr},
    {"setObjOnTop", canvas_setObjOnTop},
    {"removeObj", canvas_removeObj},
    {NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_canvas(lua_State *L) {
    luaL_register(L, LUA_CANVASLIBNAME, canvaslib);
    return 1;
}
