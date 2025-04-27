#ifndef LUA_DS_LIB_H_
#define LUA_DS_LIB_H_

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define LUA_SCREENLIBNAME "screen"
LUALIB_API int (luaopen_screen) (lua_State *L);

#define LUA_CONTROLSLIBNAME "Controls"
LUALIB_API int (luaopen_controls) (lua_State *L);

#define LUA_IMAGELIBNAME "Image"
LUALIB_API int (luaopen_image) (lua_State *L);

#define LUA_SYSTEMLIBNAME "System"
LUALIB_API int (luaopen_system) (lua_State *L);

#define LUA_MAPLIBNAME "Map"
LUALIB_API int (luaopen_map) (lua_State *L);

#define LUA_FONTLIBNAME "Font"
LUALIB_API int (luaopen_font) (lua_State *L);

#define LUA_COLORLIBNAME "Color"
LUALIB_API int (luaopen_color) (lua_State *L);

#define LUA_SCROLLMAPLIBNAME "ScrollMap"
LUALIB_API int (luaopen_scrollmap) (lua_State *L);

#define LUA_CANVASLIBNAME "Canvas"
LUALIB_API int (luaopen_canvas) (lua_State *L);

#define LUA_WIFILIBNAME "Wifi"
LUALIB_API int (luaopen_wifi) (lua_State *L);

#define LUA_NIFILIBNAME "Nifi"
LUALIB_API int (luaopen_nifi) (lua_State *L);

#define LUA_RUMBLELIBNAME "Rumble"
LUALIB_API int (luaopen_rumble) (lua_State *L);

#define LUA_MOTIONLIBNAME "Motion"
LUALIB_API int (luaopen_motion) (lua_State *L);

#define LUA_SOUNDLIBNAME "Sound"
LUALIB_API int (luaopen_sound) (lua_State *L);

#define LUA_USERLIBNAME "dsUser"
LUALIB_API int (luaopen_user) (lua_State *l);

#define LUA_TIMERLIBNAME "Timer"
LUALIB_API int (luaopen_timer) (lua_State *L);

void luaL_openDSlibs (lua_State *L);

#endif /* LUA_DS_LIB_H_ */
