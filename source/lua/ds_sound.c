/*
*
*/

#include <stdlib.h>
#include <stdio.h>

#define ds_sound_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <maxmod9.h>
#include <sys/stat.h>
#include <sys/dir.h>

#include "vars.h"

// ########## BANKS ##########

static int sound_loadBank(lua_State *L){
    char * filename = (char *)luaL_checkstring(L, 1);
    assert(L, filename != NULL, "Filename can't be null");
    struct stat file_status;
    if(stat(filename, &file_status) != 0){
        luaL_error(L, "Unable to load %s", filename);
    }
    mmInitDefault(filename);
    return 0;
}

static int sound_unloadBank(lua_State *L){
    mmInitDefaultMem((mm_addr)0);
    return 0;
}

// ########## MODS ##########

static int sound_loadMod(lua_State *L){
    int mod = luaL_checknumber(L, 1);
    assert(L, mod >= 0, "Invalid MOD number");
    mmLoad(mod);
    return 0;
}

static int sound_unloadMod(lua_State *L){
    int mod = luaL_checknumber(L, 1);
    assert(L, mod >= 0, "Invalid MOD number");
    mmUnload(mod);
    return 0;
}

static int sound_startMod(lua_State *L){
    int mod = luaL_checknumber(L, 1);
    int mode = luaL_checknumber(L, 2);
    assert(L, mod >= 0, "Invalid MOD number");
    assert(L, mode == 0 || mode == 1, "Playing mode must be PLAY_ONCE OR PLAY_LOOP");
    if(mode == 0)
        mmStart(mod, MM_PLAY_LOOP);
    else
        mmStart(mod, MM_PLAY_ONCE);
    return 0;
}

static int sound_pause(lua_State *L){
    mmPause();
    return 0;
}

static int sound_resume(lua_State *L){
    mmResume();
    return 0;
}

static int sound_stop(lua_State *L){
    mmStop();
    return 0;
}

static int sound_setPosition(lua_State *L){
    int pos = luaL_checknumber(L, 1);
    assert(L, pos >= 0, "Position must be >= 0");
    mmPosition(pos);
    return 0;
}

static int sound_isActive(lua_State *L){
    lua_pushboolean(L, mmActive());
    return 1;
}

static int sound_startJingle(lua_State *L){
    int mod = luaL_checknumber(L, 1);
    assert(L, mod >= 0, "Invalid MOD number");
    mmJingle(mod);
    return 0;
}

static int sound_setModVolume(lua_State *L){
    int vol = luaL_checknumber(L, 1);
    assert(L, vol >= 0 && vol <= 1024, "Volume must be between 0 and 1024");
    mmSetModuleVolume(vol);
    return 0;
}

static int sound_setJingleVolume(lua_State *L){
    int vol = luaL_checknumber(L, 1);
    assert(L, vol >= 0 && vol <= 1024, "Volume must be between 0 and 1024");
    mmSetJingleVolume(vol);
    return 0;
}

static int sound_setModTempo(lua_State *L){
    int tempo = luaL_checknumber(L, 1);
    assert(L, tempo >= 512 && tempo <= 2048, "Tempo must be between 512 and 2048");
    mmSetModuleTempo(tempo);
    return 0;
}

static int sound_setModPitch(lua_State *L){
    int pitch = luaL_checknumber(L, 1);
    mmSetModulePitch(pitch);
    return 0;
}

// ########## SFX ##########

static int sound_loadSFX(lua_State *L){
    int sfx = luaL_checknumber(L, 1);
    assert(L, sfx >= 0, "Invalid SFX number");
    mmLoadEffect(sfx);
    return 0;
}

static int sound_unloadSFX(lua_State *L){
    int sfx = luaL_checknumber(L, 1);
    assert(L, sfx >= 0, "Invalid SFX number");
    mmUnloadEffect(sfx);
    return 0;
}

static int sound_startSFX(lua_State *L){
    int sfx = luaL_checknumber(L, 1);
    assert(L, sfx >= 0, "Invalid SFX number");
    lua_pushnumber(L, mmEffect(sfx));
    return 1;
}

static int sound_setSFXVolume(lua_State *L){
    int handle = luaL_checknumber(L, 1);
    int vol = luaL_checknumber(L, 2);
    assert(L, vol >= 0 && vol <= 255, "Volume must be between 0 and 255");
    mmEffectVolume(handle, vol);
    return 0;
}

static int sound_setSFXPanning(lua_State *L){
    int handle = luaL_checknumber(L, 1);
    int pan = luaL_checknumber(L, 2);
    assert(L, pan >= 0 && pan <= 255, "Panning must be between 0 and 255");
    mmEffectPanning(handle, pan);
    return 0;
}

static int sound_setSFXPitch(lua_State *L){
    int handle = luaL_checknumber(L, 1);
    int pitch = luaL_checknumber(L, 2);
    mmEffectPanning(handle, pitch);
    return 0;
}

static int sound_setSFXScalePitch(lua_State *L){
    int handle = luaL_checknumber(L, 1);
    int factor = luaL_checknumber(L, 2);
    mmEffectScaleRate(handle, factor);
    return 0;
}

static int sound_stopSFX(lua_State *L){
    int handle = luaL_checknumber(L, 1);
    mmEffectCancel(handle);
    return 0;
}

static int sound_releaseSFX(lua_State *L){
    int handle = luaL_checknumber(L, 1);
    mmEffectRelease(handle);
    return 0;
}

static int sound_stopAllSFX(lua_State *L){
    mmEffectCancelAll();
    return 0;
}

static const luaL_Reg soundlib[] = {
    // Banks
    {"loadBank", sound_loadBank},
    {"unloadBank", sound_unloadBank},
    // Mods
    {"loadMod", sound_loadMod},
    {"unloadMod", sound_unloadMod},
    {"startMod", sound_startMod},
    {"pause", sound_pause},
    {"resume", sound_resume},
    {"stop", sound_stop},
    {"setPosition", sound_setPosition},
    {"isActive", sound_isActive},
    {"startJingle", sound_startJingle},
    {"setModVolume", sound_setModVolume},
    {"setJingleVolume", sound_setJingleVolume},
    {"setModTempo", sound_setModTempo},
    {"setModPitch", sound_setModPitch},
    // SFX
    {"loadSFX", sound_loadSFX},
    {"unloadSFX", sound_unloadSFX},
    {"startSFX", sound_startSFX},
    {"setSFXVolume", sound_setSFXVolume},
    {"setSFXPanning", sound_setSFXPanning},
    {"setSFXPitch", sound_setSFXPitch},
    {"setSFXScalePitch", sound_setSFXScalePitch},
    {"stopSFX", sound_stopSFX},
    {"releaseSFX", sound_releaseSFX},
    {"stopAllSFX", sound_stopAllSFX},
    {NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_sound (lua_State *L) {
    luaL_register(L, LUA_SOUNDLIBNAME, soundlib);
    return 1;
}
