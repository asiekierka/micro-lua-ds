/*
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ds_wifi_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <nds.h>
#include <ulib/ulib.h>

#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "vars.h"

//Init Wifi_init var
u8 Wifi_init = 0;
    
static int wifi_connectWFC(lua_State *L){
	bool res = false;
	if(Wifi_init == 0) res = Wifi_InitDefault(WFC_CONNECT);
	if(res) Wifi_init = 1;
	lua_pushboolean(L, res);
	return 1;
}

static int wifi_initDefault(lua_State *L){
	bool res = false;
	if(Wifi_init == 0) res = Wifi_InitDefault(INIT_ONLY);
	if(res) Wifi_init = 1;
	lua_pushboolean(L,res);
	return 1;
}

static int wifi_scanAP(lua_State *L){
	Wifi_ScanMode();
	return 0;
}

static int wifi_getNumAP(lua_State *L){
	lua_pushnumber(L, Wifi_GetNumAP());
	return 1;
}

static int wifi_newAP(lua_State *L){
	lua_newtable(L);
	lua_pushstring(L,"ssid"); //key
	lua_pushstring(L,""); //value
	lua_settable(L,-3); //enregistre la ligne
	lua_pushstring(L,"bssid");
	lua_pushstring(L,"000000000000");
	lua_settable(L,-3);
	lua_pushstring(L,"macaddr");
	lua_pushstring(L,"00:00:00:00:00:00");
	lua_settable(L,-3);
	lua_pushstring(L,"channel");
	lua_pushnumber(L,0);
	lua_settable(L,-3);
	lua_pushstring(L,"rssi");
	lua_pushnumber(L,0);
	lua_settable(L,-3);
	lua_pushstring(L,"maxrate");
	lua_pushnumber(L,0);
	lua_settable(L,-3);
	lua_pushstring(L,"protection");
	lua_pushstring(L,"NONE");
	lua_settable(L,-3);
	lua_pushstring(L,"adhoc");
	lua_pushboolean(L,false);
	lua_settable(L,-3);
	lua_pushstring(L,"active");
	lua_pushboolean(L,false);
	lua_settable(L,-3);
	return 1;
}

static int wifi_getAP(lua_State *L){
	int numAP = (int)luaL_checknumber(L,1);
	assert(L, numAP>0 && numAP<Wifi_GetNumAP()+1, "Vous demandez des infos sur une AP inexistante.");
	Wifi_AccessPoint ap;
	Wifi_GetAPData(numAP-1, &ap);
	char buffer[20];
	char macaddr[20];
	char protection[] ="NONE";
	if( ap.flags & WFLAG_APDATA_WEP ) sprintf(protection,"WEP");
	else if (ap.flags & WFLAG_APDATA_WPA) sprintf(protection,"WPA");
	bool adhoc = ap.flags & WFLAG_APDATA_ADHOC ? true:false;
	bool actif = ap.flags & WFLAG_APDATA_ACTIVE ? true:false;
    //creation du tableau
	lua_newtable(L);
	lua_pushstring(L,"ssid"); //key
	lua_pushstring(L,ap.ssid); //value
	lua_settable(L,-3); //enregistre la ligne
	lua_pushstring(L,"bssid");
	sprintf(buffer,"%02x%02x%02x%02x%02x%02x",
		ap.bssid[0],ap.bssid[1],ap.bssid[2],ap.bssid[3],ap.bssid[4],ap.bssid[5]);
	lua_pushstring(L,buffer);
	lua_settable(L,-3);
	lua_pushstring(L,"macaddr");
	sprintf(macaddr,"%02x:%02x:%02x:%02x:%02x:%02x",
		ap.macaddr[0],ap.macaddr[1],ap.macaddr[2],ap.macaddr[3],ap.macaddr[4],ap.macaddr[5]);
	lua_pushstring(L,macaddr);
	lua_settable(L,-3);
	lua_pushstring(L,"channel");
	lua_pushnumber(L,ap.channel);
	lua_settable(L,-3);
	lua_pushstring(L,"rssi");
	lua_pushnumber(L,ap.rssi);
	lua_settable(L,-3);
	lua_pushstring(L,"maxrate");
	lua_pushnumber(L,ap.maxrate);
	lua_settable(L,-3);
	lua_pushstring(L,"protection");
	lua_pushstring(L,protection);
	lua_settable(L,-3);
	lua_pushstring(L,"adhoc");
	lua_pushboolean(L,adhoc);
	lua_settable(L,-3);
	lua_pushstring(L,"active");
	lua_pushboolean(L,actif);
	lua_settable(L,-3);
	return 1;
}

static int wifi_connectAP(lua_State *L){
	u8 wepmod = 0;
	Wifi_AccessPoint ap;
	char wepkey[30];
	u8 i, j;
	u8 numAP = 0;
	u8 numkey = 0;
	unsigned char *key = NULL;
	char rep;
	u8 ltype = lua_type(L, 1);
	if(ltype == LUA_TNUMBER)
	{
		numAP = (u8)luaL_checknumber(L,1) -1;
		numkey = (u8)luaL_checknumber(L,2);
		strcpy((char *)key,(char *)luaL_checkstring(L,3));
		Wifi_GetAPData(numAP, &ap);
	}
	else
	{
		Wifi_AccessPoint apdata;
		sprintf(apdata.ssid,"%s",(char *)luaL_checkstring(L,1));
		apdata.ssid_len = strlen(apdata.ssid);
		unsigned char *buff = (unsigned char*)luaL_checkstring(L,2);
		i=0;
		while(*buff) {
			j=0;
			if(*buff>='0' && *buff<='9') j=*buff-'0';
			if(*buff>='A' && *buff<='F') j=*buff-'A'+10;
			if(i&1) apdata.bssid[i/2] = (apdata.bssid[i/2]&0xF0)|j; 
			else apdata.bssid[i/2] = (apdata.bssid[i/2]&0xF)|(j<<4);
			i++;
			buff++;
		}
		apdata.bssid[i/2] = 0;
		apdata.channel = (u8)luaL_checknumber(L,3);
		numkey = (u8)luaL_checknumber(L,4);
		strcpy((char *)key,(char *)luaL_checkstring(L,5));
		//i=Wifi_FindMatchingAP(1,&apdata,&ap);
	}
	numkey = numkey-1;
	if(strlen((char *)key) < 2) rep = Wifi_ConnectAP(&ap,wepmod,0,0); //wepmod = 0 
	else
	{
		char weplength = strlen((char *)key);
		if(weplength < 12) wepmod = 1;
		else wepmod = 2;
		if(weplength == 10 || weplength == 26)
		{	
			i=0;
			while(*key) 
			{
				j=0;
				if(*key>='0' && *key<='9') j=*key-'0';
				if(*key>='A' && *key<='F') j=*key-'A'+10;
				if(i&1) wepkey[i/2] = (wepkey[i/2]&0xF0)|j; 
				else wepkey[i/2] = (wepkey[i/2]&0xF)|(j<<4);
				i++;
				key++;
			}
			wepkey[i/2] = 0;
		}
		else strcpy(wepkey,(char *)key);
		rep = Wifi_ConnectAP(&ap,wepmod,numkey,(u8 *)wepkey);
	}
	if(rep == 0)
	{
		//attente état de connexion
		int wifiStatus = ASSOCSTATUS_DISCONNECTED;
		while(wifiStatus != ASSOCSTATUS_ASSOCIATED)
		{
			wifiStatus = Wifi_AssocStatus(); // check status

			if(wifiStatus == ASSOCSTATUS_CANNOTCONNECT)
			{
				lua_pushnumber(L,-1);
				return 1;
			}
			swiWaitForVBlank();
		}  
	}
	lua_pushnumber(L,rep);
	return 1;
}

static int wifi_status(lua_State *L){
	int status = Wifi_AssocStatus();
	lua_pushstring(L, ASSOCSTATUS_STRINGS[status]);
	return 1;
}

static int wifi_stop(lua_State *L){
	Wifi_DisableWifi();
	return 0;
}

static int wifi_disconnect(lua_State *L){
	Wifi_DisconnectAP();
	return 0;
}

/* inutile
static int wifi_getLocalIP(lua_State *L){
	struct in_addr ip;
	ip.s_addr = Wifi_GetIP();
	lua_pushstring(L, inet_ntoa(ip));
	return 1;
}
*/

static int wifi_getLocalConf(lua_State *L){
	int choix = (int)luaL_checknumber(L,1);
	assert(L, choix>=1 && choix <= 6, "Arg1 must be between 1 and 6");
	struct in_addr ip;
	struct in_addr gate;
	struct in_addr submask;
	struct in_addr dns1;
	struct in_addr dns2;
	unsigned char *buffer = NULL;
	ip = Wifi_GetIPInfo(&gate, &submask, &dns1, &dns2);
	Wifi_GetData(WIFIGETDATA_MACADDRESS, 6, buffer);
	// Choix = 1:ip  2:gateway  3:subnetMask  4:DNS1  5:DNS2  6:MACADDR
	switch(choix){
		case 1: lua_pushstring(L, inet_ntoa(ip));
			break;
		case 2: lua_pushstring(L, inet_ntoa(gate));
			break;
		case 3: lua_pushstring(L, inet_ntoa(submask));
			break;
		case 4: lua_pushstring(L, inet_ntoa(dns1));
			break;
		case 5: lua_pushstring(L, inet_ntoa(dns2));
			break;
		case 6: lua_pushstring(L, (char *)buffer);
			break;
	}
	return 1;
}

static int wifi_resetIP(lua_State *L){
	Wifi_SetIP(0,0,0,0,0);
	return 0;
}

static int wifi_autoConnectWFC(lua_State *L){
	Wifi_AutoConnect();
	return 0;
}

static int wifi_setLocalIP(lua_State *L){
	char * ip = (char *)luaL_checkstring(L,1);
	char * gateway = (char *)luaL_checkstring(L,2);
	char * subnetmask = (char *)luaL_checkstring(L,3);
	char * dns1 = (char *)luaL_checkstring(L,4);
	char * dns2 = (char *)luaL_checkstring(L,5);
	struct in_addr iph, gate, mask, DNS1, DNS2;
	assert(L, ip != NULL, "IP can't be null.");
	assert(L, gateway != NULL, "Gateway can't be null"); 
	if(subnetmask == NULL) strcpy(subnetmask,"255.255.255.0");
	if(dns1 == NULL) strcpy(dns1,"0.0.0.0");
	if(dns2 == NULL) strcpy(dns2,"0.0.0.0");
	inet_aton(ip, &iph);
	inet_aton(gateway, &gate);
	inet_aton(subnetmask, &mask);
	inet_aton(dns1, &DNS1);
	inet_aton(dns2, &DNS2);
	Wifi_SetIP(iph.s_addr,gate.s_addr,mask.s_addr,DNS1.s_addr,DNS2.s_addr);
	return 0;
}

static int wifi_createTCPSocket(lua_State *L){
	char * url = (char *)luaL_checkstring(L, 1);
	int port = (int)luaL_checknumber(L, 2);
	assert(L, url != NULL, "Url can't be null");
	assert(L, port >= 0 && port <= 65565, "Port number must be betweem 0 and 65565");
	struct hostent * myhost = gethostbyname(url);
	int my_socket;
	my_socket = socket(AF_INET, SOCK_STREAM, 0);
	assert(L, my_socket >= 0, "Impossible to create the socket");
	struct sockaddr_in sain;
	sain.sin_family = AF_INET;
	sain.sin_port = htons(port);
	sain.sin_addr.s_addr= *((unsigned long *)(myhost->h_addr_list[0]));
	connect(my_socket,(struct sockaddr *)&sain, sizeof(sain));
	lua_pushnumber(L, my_socket);
	return 1;
}

static int wifi_createUDPSocket(lua_State *L){
	char * url = (char *)luaL_checkstring(L, 1);
	int port = (int)luaL_checknumber(L, 2);
	assert(L, url != NULL, "Url can't be null");
	assert(L, port >= 0 && port <= 65565, "Port number must be betweem 0 and 65565");
	struct hostent * myhost = gethostbyname(url);
	int my_socket;
	my_socket = socket(AF_INET, SOCK_DGRAM, 0);
	assert(L, my_socket >= 0, "Impossible to create the socket");
	struct sockaddr_in sain;
	sain.sin_family = AF_INET;
	sain.sin_port = htons(port);
	sain.sin_addr.s_addr= *((unsigned long *)(myhost->h_addr_list[0]));
	connect(my_socket,(struct sockaddr *)&sain, sizeof(sain));
	lua_pushnumber(L, my_socket);
	return 1;
}

static int wifi_closeSocket(lua_State *L){
	int my_socket = (int)luaL_checknumber(L, 1);
	assert(L, my_socket >= 0, "Invalid socket");
	shutdown(my_socket, 0);
	closesocket(my_socket);
	return 0;
}

static int wifi_send(lua_State *L){
	int my_socket = (int)luaL_checknumber(L, 1);
	unsigned int length;
	char * buffer = (char *)luaL_checklstring(L, 2, &length);
	assert(L, my_socket >= 0, "Invalid socket");
	assert(L, buffer != NULL, "Buffer can't be null");
	send(my_socket, buffer, length, 0);
	return 0;
}

static int wifi_checkData(lua_State *L){
	unsigned long available;
	int my_socket = (int)luaL_checknumber(L, 1);
	assert(L, my_socket >= 0, "Invalid socket");
	ioctl(my_socket, FIONREAD, &available);
	lua_pushnumber(L, available);
	return 1;
}

static int wifi_receive(lua_State *L){
	int my_socket = (int)luaL_checknumber(L, 1);
	int length = (int)luaL_checknumber(L, 2);
	assert(L, my_socket >= 0, "Invalid socket");
	assert(L, length > 0, "Length must be > 0");
	char buffer[length];
	int recv_len = recv(my_socket, buffer, length, 0);
	if(recv_len > 0){
		lua_pushlstring(L, buffer,recv_len);
		return 1;
	}else{
		return 0;
	}	
}

static const luaL_Reg wifilib[] = {
	{"initDefault", wifi_initDefault},
	{"connectWFC", wifi_connectWFC},
	{"autoConnectWFC", wifi_autoConnectWFC},
	{"getLocalConf", wifi_getLocalConf},
	{"checkData", wifi_checkData},
	{"resetIP", wifi_resetIP},
	{"scanAP", wifi_scanAP},
	{"getNumAP", wifi_getNumAP},
	{"getAP", wifi_getAP},
	{"newAP", wifi_newAP},
	{"stop", wifi_stop},
	{"connectAP", wifi_connectAP},
	{"status", wifi_status},
	{"disconnect", wifi_disconnect},
//	{"getLocalIP", wifi_getLocalIP},
	{"setLocalIP", wifi_setLocalIP},
	{"createTCPSocket", wifi_createTCPSocket},
	{"createUDPSocket", wifi_createUDPSocket},
	{"closeSocket", wifi_closeSocket},
	{"send", wifi_send},
	{"receive", wifi_receive},
	{NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_wifi (lua_State *L) {
  luaL_register(L, LUA_WIFILIBNAME, wifilib);
  return 1;
}





