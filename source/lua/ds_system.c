/*
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <nds.h>

#define ds_system_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <fat.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <ulib/ulib.h>

#include "vars.h"

#define lua_geti(L, index, i) \
    lua_pushnumber(L, i); \
    lua_gettable(L, index);
#define lua_seti(L, index, i) \
    lua_pushnumber(L, i); \
    lua_pushvalue(L, -2); \
    lua_settable(L, index); \
    lua_pop(L, 1);
#define isRelative(path) \
    path[0] != '/' && \
    !strncmp("nitro:", (path), 6) && !strncmp("fat:", (path), 4)

static int system_currentVramUsed(lua_State *L) {
	lua_pushnumber(L, (unsigned int) ulGetTexVramUsedMemory());
	return 1;
}

static int system_currentVramFree(lua_State *L){
	lua_pushnumber(L, (unsigned int) ulGetTexVramAvailMemory());
	return 1;
}

//cette fonction retourne aussi un chiffre bizare
static int system_currentPalUsed(lua_State *L){
	lua_pushnumber(L, (unsigned int) ulGetTexPalUsedMemory());
	return 1;
}

static int system_currentPalFree(lua_State *L){
	lua_pushnumber(L, (unsigned int) ulGetTexPalAvailMemory());
	return 1;
}

static int system_currentDirectory(lua_State *L){
	char path[256];
    getcwd(path, 255);
	lua_pushstring(L, path);
	return 1;
}

static int system_changeDirectory(lua_State *L){
	char * dir = (char *)luaL_checkstring(L, 1);
	chdir(dir);
	return 0;
}

static int system_remove(lua_State *L){
	char * dir = (char *)luaL_checkstring(L, 1);
	remove(dir);
	return 0;
}

static int system_rename(lua_State *L){
	char * dir1 = (char *)luaL_checkstring(L, 1);
	char * dir2 = (char *)luaL_checkstring(L, 2);
	rename(dir1, dir2);
	return 0;
}

static int system_makeDirectory(lua_State *L){
	char * dir = (char *)luaL_checkstring(L, 1);
	mkdir(dir, 777);
	return 0;
}

int mustSwap(lua_State *L, int dirList, int i) {
    // Special function to sort the final table placing folders first
    int isDir1, isDir2, nameCmp;
    char *name1 = malloc(256*sizeof(char)), *name2 = malloc(256*sizeof(char));
    
    lua_geti(L, dirList, i);
    lua_getfield(L, -1, "isDir");
    isDir1 = lua_toboolean(L, -1);
    lua_getfield(L, -2, "name");
    strcpy(name1, lua_tostring(L, -1));
    lua_pop(L, 3);                      // Removes elem[i], its 'isDir' and its 'name'
    lua_geti(L, dirList, i+1);
    lua_getfield(L, -1, "isDir");
    isDir2 = lua_toboolean(L, -1);
    lua_getfield(L, -2, "name");
    strcpy(name2, lua_tostring(L, -1));
    lua_pop(L, 3);                      // Idem
    
    nameCmp = strcmp(strlwr(name1), strlwr(name2));
    
    free(name1);
    free(name2);
    
    return (!isDir1 && isDir2) || ((nameCmp > 0) && (isDir1 == isDir2));
}

static int system_listDirectory(lua_State *L) {
    const char *askedPath = luaL_checkstring(L, 1);
    char path[PATH_MAX] = "";           // Full path
    struct dirent *entry;
    struct stat st;
    DIR *dir;
    // entryName points to the end of the absolute path leading to the folder (and to the start of the name of the entry in the path)
    char *entryName, *end = path + sizeof(path) - 1;
    int i = 1, nbItems;
    int dirList;            // Hold the index of the resulting table in the stack
    int swapped = 1;
    
    if (isRelative(askedPath))
        if (getcwd(path, sizeof(path)) == NULL) luaL_error(L, "couldn't get the current working directory");
    if (strlen(path) + strlen(askedPath) >= sizeof(path)) luaL_error(L, "path is too long");
    strcpy(path + strlen(path), askedPath);
    
    // We will append each entry's name there to the path
    entryName = path + strlen(path);
    
    // Make sure there's a trailing slash
    if (entryName[-1] != '/' && entryName < end)
        *entryName++ = '/';     // Also move the beginning of the entry's name to the right
    
    if ((dir = opendir(path)) == NULL) luaL_error(L, "couldn't open folder %s", path);
    
    lua_newtable(L);        // This is the return table listing the folder
    dirList = lua_gettop(L);
    
    while ((entry = readdir(dir)) != NULL) {    // Loop through the folder
        if (entryName + strlen(entry->d_name) >= end) luaL_error(L, "path is too long");
        
        strcpy(entryName, entry->d_name);
        
        // stat wouldn't work properly on a folder in EFS
        if (entry->d_type != DT_DIR && stat(path, &st) != 0) luaL_error(L, "couldn't stat the entry %s", path);
        
        // From now everything is okay
        lua_pushnumber(L, i);                   // Will be the index of this element
        lua_newtable(L);                        // Entry's table
        // Fill the element
        lua_pushstring(L, entry->d_name);
        lua_setfield(L, -2, "name");
        lua_pushboolean(L, entry->d_type == DT_DIR);
        lua_setfield(L, -2, "isDir");
        if (entry->d_type != DT_DIR) { lua_pushnumber(L, st.st_size); } else { lua_pushnumber(L, 0); }
        lua_setfield(L, -2, "size");
        // Add the element to the resulting table
        lua_settable(L, dirList);
        i++;
    }
    
    closedir(dir);
    
    nbItems = i - 1;
    
    // Now we need to sort the table (we will use modified bubble sorting)
    // We use a custom function to put folders at the top of the table
    while (swapped) {
        swapped = 0;
        
        for (i = 1; i <= nbItems-1; i++) {
            if (mustSwap(L, dirList, i)) {
                lua_geti(L, dirList, i);
                lua_geti(L, dirList, i+1);
                lua_seti(L, dirList, i);
                lua_seti(L, dirList, i+1);
                swapped = 1;
            }
        }
        
        nbItems--;
    }
    
    return 1;
}

static int system_setLedBlinkMode(lua_State *L) {
	int mode = (int)luaL_checknumber(L, 1);
	assert(L, mode == PM_LED_ON || mode == PM_LED_SLEEP || mode == PM_LED_BLINK, "Bad led blink mode");
	ledBlink(mode);
	
	return 0;
}

static int system_systemShutDown(lua_State *L) {
	systemShutDown();
	
	return 0;
}

static int system_systemSleep(lua_State *L) {
	systemSleep();
	
	return 0;
}

static const luaL_Reg systemlib[] = {
	{"currentDirectory", system_currentDirectory},
	{"changeDirectory", system_changeDirectory},
	{"remove", system_remove},
	{"rename", system_rename},
	{"makeDirectory", system_makeDirectory},
	{"listDirectory", system_listDirectory},
	{"CurrentVramUsed",system_currentVramUsed},
	{"CurrentVramFree",system_currentVramFree},
	{"CurrentPalUsed",system_currentPalUsed},
	{"CurrentPalFree",system_currentPalFree},
	{"setLedBlinkMode", system_setLedBlinkMode},
	{"shutDown", system_systemShutDown},
	{"sleep", system_systemSleep},
	{NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_system (lua_State *L) {
    luaL_register(L, LUA_SYSTEMLIBNAME, systemlib);
	
    return 1;
}
