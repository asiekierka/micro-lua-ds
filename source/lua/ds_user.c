#define ds_user_c
#define LUA_LIB
 
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
  
#include "vars.h"
   
static int user_theme(lua_State* l) {
    lua_pushnumber(l, PersonalData->theme);
    
    return 1;
}

static int user_birthMonth(lua_State* l) {
    lua_pushnumber(l, PersonalData->birthMonth);
    
    return 1;
}

static int user_birthDay(lua_State* l) {
    lua_pushnumber(l, PersonalData->birthDay);
    
    return 1;
}

static int user_name(lua_State* l) {
    char name[10 * 4 + 1];
    int ret = utf16_to_utf8(name, sizeof(name), PersonalData->name, PersonalData->nameLen * 2);
    if (ret <= 0) name[0] = '\0';
    lua_pushstring(l, name);
    return 1;
}

static int user_nameLength(lua_State* l) {
    lua_pushnumber(l, PersonalData->nameLen);
    return 1;
}

static int user_message(lua_State* l) {
    char msg[10 * 4 + 1];
    int ret = utf16_to_utf8(msg, sizeof(msg), PersonalData->message, PersonalData->messageLen * 2);
    if (ret <= 0) msg[0] = '\0';
    lua_pushstring(l, msg);
    return 1;
}

static int user_messageLength(lua_State* l) {
    lua_pushnumber(l, PersonalData->messageLen);

    return 1;
}

static int user_alarmHour(lua_State* l) {
    lua_pushnumber(l, PersonalData->alarmHour);

    return 1;
}

static int user_alarmMinute(lua_State* l) {
    lua_pushnumber(l, PersonalData->alarmMinute);

    return 1;
}

static int user_language(lua_State* l) {
    lua_pushnumber(l, PersonalData->language);

    return 1;
}

static int user_GBAScreen(lua_State* l) {
    if (PersonalData->gbaScreen) {
        lua_pushboolean(l, 1);
    } else {
        lua_pushboolean(l, 0);
    }
    
    return 1;
}

static int user_defaultBrightness(lua_State *l) {
    lua_pushnumber(l, PersonalData->defaultBrightness);

    return 1;
}

static int user_autoMode(lua_State *l) {
    if (PersonalData->autoMode) {
        lua_pushboolean(l, 1);
    } else {
        lua_pushboolean(l, 0);
    }

    return 1;
}

static const luaL_Reg user[] = {
    {"getColor", user_theme},
    {"getBirthMonth", user_birthMonth},
    {"getBirthDay", user_birthDay},
    {"getName", user_name},
    {"getNameLength", user_nameLength},
    {"getMessage", user_message},
    {"getMessageLength", user_messageLength},
    {"getAlarmHour", user_alarmHour},
    {"getAlarmMinute", user_alarmMinute},
    {"getLanguage", user_language},
    {"getGBAScreen", user_GBAScreen},
    {"getDefaultBrightness", user_defaultBrightness},
    {"getAutoMode", user_autoMode},
    {NULL, NULL}
};

LUALIB_API int luaopen_user(lua_State* l) {
    luaL_register(l, LUA_USERLIBNAME, user);
    
    lua_getglobal(l, "dsUser");
    user_theme(l);
    lua_setfield(l, 2, "color");
    user_birthMonth(l);
    lua_setfield(l, 2, "birthMonth");
    user_birthDay(l);
    lua_setfield(l, 2, "birthDay");
    user_name(l);
    lua_setfield(l, 2, "name");
    user_nameLength(l);
    lua_setfield(l, 2, "nameLength");
    user_message(l);
    lua_setfield(l, 2, "message");
    user_messageLength(l);
    lua_setfield(l, 2, "messageLength");
    user_alarmHour(l);
    lua_setfield(l, 2, "alarmHour");
    user_alarmMinute(l);
    lua_setfield(l, 2, "alarmMinute");
    user_language(l);
    lua_setfield(l, 2, "language");
    user_GBAScreen(l);
    lua_setfield(l, 2, "GBAScreen");
    user_defaultBrightness(l);
    lua_setfield(l, 2, "defaultBrightness");
    user_autoMode(l);
    lua_setfield(l, 2, "autoMode");
    lua_pop(l, 1);

    return 1;
}
