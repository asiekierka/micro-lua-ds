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
#include <sys/stat.h>
#include <sys/dir.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <ulib/ulib.h>

#include "vars.h"

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

/*static int system_listDirectory(lua_State *L){
	if(dirListed == 0){
		dirListed = 1;
		dirname = (char *)luaL_checkstring(L, 1);
		pdir = opendir(dirname);
	}
	char filename[768];
	if((pent=readdir(pdir))!=NULL){
		stat(pent->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)){ // Is directory
			sprintf(filename, "*%s", pent->d_name);
			lua_pushstring(L, filename);
		}else{ // Is not directory
			lua_pushstring(L, pent->d_name);
		}
	}else{ // End
		dirListed = 0;
		closedir(pdir);
		lua_pushstring(L, "##");
	}
	return 1;
}*/

static int system_listDirectory(lua_State *L){
	struct dirent *pent;
	struct stat statbuf;
	bool cont = true;
	struct filestat {
		char name[255];
		bool isDir;
		int size;
	};
	struct listtri{
		struct filestat fstat;
		struct listtri *suiv;
	} *prem, *prec, *tmp, *tmp2;
	prem = NULL;
	prec = NULL;
	tmp = NULL;
	char str1[255];
	char str2[255];
	char *dirname = (char *)luaL_checkstring(L,1);
	DIR *pdir = opendir(dirname);
	u8 index = 0;
	if(pdir !=NULL){
		while((pent=readdir(pdir))!=NULL){
			stat(pent->d_name,&statbuf);
			tmp = malloc(sizeof(struct listtri));
			strcpy(tmp->fstat.name,pent->d_name);
			tmp->fstat.isDir = S_ISDIR(statbuf.st_mode);
			tmp->fstat.size = statbuf.st_size;
			tmp->suiv = NULL;
			if(prem == NULL) prem = tmp;
			else{
				tmp2 = prem;
				prec = prem;
				cont = true;
				while(cont){
					strcpy(str1,tmp2->fstat.name);
					strcpy(str2,tmp->fstat.name);
					if(strcmp(strlwr(str1),strlwr(str2)) > 0){
						if(prec != prem) prec->suiv = tmp;
						else prem = tmp;
						tmp->suiv = tmp2;
						cont = false;
					}
					else{
						prec = tmp2;
						tmp2 = tmp2->suiv;
					}
					if(tmp2 == NULL){
						prec->suiv = tmp;
						cont = false;
					}
				}
			}                       
		}
		closedir(pdir);
		tmp = prem;
		lua_newtable(L);
		while(tmp != NULL){  //On commence par les dossiers
			if(tmp->fstat.isDir){
				lua_pushnumber(L,index++); //clef 
				lua_newtable(L);        //value = table
				lua_pushstring(L,"name"); //key
				lua_pushstring(L,tmp->fstat.name); //value
				lua_settable(L,-3); //enregistre la ligne
				lua_pushstring(L,"isDir"); //key
				lua_pushboolean(L,tmp->fstat.isDir); //value
				lua_settable(L,-3); //enregistre la ligne
				lua_pushstring(L,"size"); //key
				lua_pushnumber(L,tmp->fstat.size); //value
				lua_settable(L,-3); //enregistre la ligne
				lua_settable(L,-3);
			}
			tmp = tmp->suiv;
		}
		tmp = prem;
		while(tmp != NULL){  // On fait la meme pour les fichiers
			if(!tmp->fstat.isDir){
				lua_pushnumber(L,index++); //clef 
				lua_newtable(L);        //value = table
				lua_pushstring(L,"name"); //key
				lua_pushstring(L,tmp->fstat.name); //value
				lua_settable(L,-3); //enregistre la ligne
				lua_pushstring(L,"isDir"); //key
				lua_pushboolean(L,tmp->fstat.isDir); //value
				lua_settable(L,-3); //enregistre la ligne
				lua_pushstring(L,"size"); //key
				lua_pushnumber(L,tmp->fstat.size); //value
				lua_settable(L,-3); //enregistre la ligne
				lua_settable(L,-3);
			}
			prec = tmp;
			tmp = tmp->suiv;
			free(prec);
		}
		return 1;
	}
	else return 0;
}

static int system_getCurrentTime(lua_State *L){
	int type = (int)luaL_checknumber(L, 1);
	time_t unixTime = time(0);
	struct tm* timeStruct = localtime((const time_t *)&unixTime);
	int ret = 0;
	switch(type){
		case TIME_YEAR:
			ret = timeStruct->tm_year + 1900;
		break;
		case TIME_MONTH:
			ret = timeStruct->tm_mon + 1;
		break;
		case TIME_DAY:
			ret = timeStruct->tm_mday;
		break;
		case TIME_WEEKDAY:
			ret = timeStruct->tm_wday;
		break;
		case TIME_YEARDAY:
			ret = timeStruct->tm_yday;
		break;
		case TIME_HOUR:
			ret = timeStruct->tm_hour;
		break;
		case TIME_MINUTE:
			ret = timeStruct->tm_min;
		break;
		case TIME_SECOND:
			ret = timeStruct->tm_sec;
		break;
		default:
			luaL_error(L, "Bad parameter");
		break;
	}
	timeStruct = NULL;
	lua_pushnumber(L, ret);
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
	{"getCurrentTime", system_getCurrentTime},
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
