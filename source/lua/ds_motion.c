/*
*
*/

#include <stdlib.h>
#include <stdio.h>

#define ds_motion_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <nds.h>

#include "vars.h"

// Since motion_init is already used, we slightly break a rule of consistence.
static int motion_initlib(lua_State *L){
    lua_pushboolean(L, motion_init());
    return 1;
}

static int motion_calibrate(lua_State *L){
    motion_set_offs_x();
    motion_set_offs_y();
    motion_set_offs_gyro();
    motion_set_sens_z(motion_read_z());
    return 0;
}

static int motion_readX(lua_State *L){
    lua_pushnumber(L, motion_read_x());
    return 1;
}

static int motion_readY(lua_State *L){
    lua_pushnumber(L, motion_read_y());
    return 1;
}

static int motion_readZ(lua_State *L){
    lua_pushnumber(L, motion_read_z());
    return 1;
}

static int motion_accelerationX(lua_State *L){
    lua_pushnumber(L, motion_acceleration_x());
    return 1;
}

static int motion_accelerationY(lua_State *L){
    lua_pushnumber(L, motion_acceleration_y());
    return 1;
}

static int motion_accelerationZ(lua_State *L){
    lua_pushnumber(L, motion_acceleration_z());
    return 1;
}

static int motion_readGyro(lua_State *L){
    lua_pushnumber(L, motion_read_gyro());
    return 1;
}

// Since motion_rotation is already used, we slightly break a rule of consistence.
static int motion_librotation(lua_State *L){
    lua_pushnumber(L, motion_rotation());
    return 1;
}

static const luaL_Reg motionlib[] = {
    {"init", motion_initlib},
    {"calibrate", motion_calibrate},
    {"readX", motion_readX},
    {"readY", motion_readY},
    {"readZ", motion_readZ},
    {"accelerationX", motion_accelerationX},
    {"accelerationY", motion_accelerationY},
    {"accelerationZ", motion_accelerationZ},
    {"readGyro", motion_readGyro},
    {"rotation", motion_librotation},
    {NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_motion(lua_State *L) {
    luaL_register(L, LUA_MOTIONLIBNAME, motionlib);
    return 1;
}
