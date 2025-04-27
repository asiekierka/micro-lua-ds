/*
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ds_nifi_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <nds.h>
#include <ulib/ulib.h>

#include <dswifi9.h>

#include "vars.h"

char trame[4096];
int taille;
bool newMess;

void Handler(int packetID, int readLength){
	//static int bytesRead;
	//static char buffer[4096];
	char pref[3];
	char tt[5];
	int i;
	//unsigned char *buff;
	//taille=0;
	//bytesRead = 
	Wifi_RxRawReadPacket(packetID, readLength, (unsigned short *)trame);
	for (i=0;i<2;i++) pref[i] = trame[33+i];
	pref[2] = 0;
	for(i=0;i<4;i++) tt[i] = trame[35+i];
	tt[4] = 0;
	//strcpy(trame, buffer);
	if(strcmp(pref,"µL")){
		//sprintf(tt,"%c%c%c%c\n",entete[2],entete[3],entete[4],entete[5]);
		taille = atoi(tt);
		newMess = true;	
	}
}

static int nifi_init(lua_State *L){
	bool res = false;
	newMess = false;
	taille = 0;
	//if(Wifi_init == 0) 
	res = Wifi_InitDefault(INIT_ONLY);
	char canal = luaL_checknumber(L,1);
	if(canal<1 && canal >13) canal = 10;
	if(res){
		//Wifi_init = 1;
		Wifi_SetPromiscuousMode(true);
		Wifi_EnableWifi();
		Wifi_RawSetPacketHandler(Handler);
		Wifi_SetChannel(canal);
		swiWaitForVBlank();
	}
	lua_pushboolean(L,res);
	return 1;
}

static int nifi_getTrame(lua_State *L){
	lua_pushlstring(L,trame,4096);
	return 1;
}

static int nifi_readMessage(lua_State *L){
	lua_pushlstring(L,trame+39,taille);
	lua_pushnumber(L,taille);
	newMess = false;
	return 2;
}

static int nifi_checkMessage(lua_State *L){
	lua_pushboolean(L, newMess);
	return 1;
}

static int nifi_changeChannel(lua_State *L){
	char canal=luaL_checknumber(L, 1);
	assert(L, canal>0 && canal<14, "Channel must be between 1 and 13");
	Wifi_SetChannel(canal);
	return 0;
}

static int nifi_stop(lua_State *L){
	Wifi_DisableWifi();
	return 0;
}

static int nifi_sendMessage(lua_State *L){
	unsigned int length;
	static char mess[2053];
	char *buffer = (char *)luaL_checklstring(L, 1, &length);
	assert(L, length<2048, "Data must be under 2048 char.");
	//int nb1=(int)length/256;
	//sprintf(mess,"µL%c%c%s\n",nb1,length-(nb1*256),buffer);
	sprintf(mess,"µL%04d%s\n",length,buffer);
	//mess[0]='µ';
	//mess[1]='L';
	//mess[2]=(int)length/256;
	//mess[3]=length-(mess[2]*256);
	//luaL_error(L, "[33]=%c ... [34]=%c",trame[33],trame[34]);
	//strcat((char *)mess+3, buffer);
	if(Wifi_RawTxFrame(length+7, 0x0014, (unsigned short *)mess) !=0)
		luaL_error(L, "Error sending: %s", buffer);
	return 0;
}

static const luaL_Reg nifilib[] = {
	{"init", nifi_init},
	{"getTrame", nifi_getTrame},
	{"readMessage", nifi_readMessage},
	{"sendMessage", nifi_sendMessage},
	{"checkMessage", nifi_checkMessage},
	{"changeChannel", nifi_changeChannel},
	{"stop", nifi_stop},
	{NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_nifi (lua_State *L) {
  luaL_register(L, LUA_NIFILIBNAME, nifilib);
  return 1;
}
