#include "ds_lib.h"

/*
** libs from MicroLua
*/
static const luaL_Reg loadedDSlibs[] = {
  {LUA_SCREENLIBNAME, luaopen_screen},
  {LUA_CONTROLSLIBNAME, luaopen_controls},
  {LUA_IMAGELIBNAME, luaopen_image},
  {LUA_SYSTEMLIBNAME, luaopen_system},
  {LUA_MAPLIBNAME, luaopen_map},
  {LUA_FONTLIBNAME, luaopen_font},
  {LUA_COLORLIBNAME, luaopen_color},
  {LUA_SCROLLMAPLIBNAME, luaopen_scrollmap},
  {LUA_CANVASLIBNAME, luaopen_canvas},
  {LUA_WIFILIBNAME, luaopen_wifi},
  {LUA_NIFILIBNAME, luaopen_nifi},
  {LUA_RUMBLELIBNAME, luaopen_rumble},
  {LUA_MOTIONLIBNAME, luaopen_motion},
  {LUA_SOUNDLIBNAME, luaopen_sound},
  {LUA_USERLIBNAME, luaopen_user},
  {LUA_TIMERLIBNAME, luaopen_timer},
  {NULL, NULL}
};

void luaL_openDSlibs (lua_State *L) {
  for (const luaL_Reg *lib = loadedDSlibs; lib->func; lib++) {
    luaL_requiref(L, lib->name, lib->func, 1);
    lua_pop(L, 1);
  }
}

