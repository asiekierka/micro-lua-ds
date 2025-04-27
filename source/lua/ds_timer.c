/*
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <nds.h>
#include <time.h>

#define ds_timer_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "vars.h"

#define TIMER_META "TimerMetatable"
// This is the speed of our timer (about 32728.5 Hz)
#define TIMER_CLOCK (BUS_CLOCK/1024)
// We will be using timer 0
// Timer 3 may be used by dswifi
#define TIMER_ID 0

typedef enum {
	timerStatePause,			// Timer is currently not running
	timerStateRunning			// Timer is currently running
} TimerStates;

typedef struct TimerObj {
	TimerStates state;
	lua_Number initialTicks;		// Number of ticks when starting the timer
	lua_Number ticksOnPause;		// Number of ticks when pausing the timer
} Timer;

// Counting the ticks produced by the hardware timer TIMER_ID
lua_Number ticks = 0;

static int timer_new(lua_State *L) {
	// Initialise the timer with an initial time (in ms)
	// This is simulated by faking the timer has been paused
	lua_Number initialTime = 0;
	if (lua_gettop(L) > 0) initialTime = luaL_checknumber(L, 1);
	
	ticks += timerElapsed(TIMER_ID);
	
	Timer* timer = (Timer*)lua_newuserdata(L, sizeof(Timer));
	timer->state = timerStatePause;
	timer->initialTicks = 0;
	timer->ticksOnPause = initialTime * TIMER_CLOCK / 1000;
	
	luaL_setmetatable(L, TIMER_META);
	
	return 1;
}

static int timer_time(lua_State *L) {
	Timer* timer = (Timer*)lua_touserdata(L, 1);
	lua_Number timerTicks = 0;
	
	ticks += timerElapsed(TIMER_ID);
	
	if (timer->state == timerStateRunning)
		timerTicks = ticks - timer->initialTicks;
	else if (timer->state == timerStatePause)
		timerTicks = timer->ticksOnPause - timer->initialTicks;
	
	lua_pushnumber(L, timerTicks / TIMER_CLOCK * 1000);
	
	return 1;
}

static int timer_start(lua_State *L) {
	Timer* timer = (Timer*)lua_touserdata(L, 1);
	
	ticks += timerElapsed(TIMER_ID);
	
	if (timer->state == timerStatePause) {
		timer->state = timerStateRunning;
		timer->initialTicks = ticks - timer->ticksOnPause + timer->initialTicks;
		timer->ticksOnPause = 0;
	}
	
	return 0;
}

static int timer_stop(lua_State *L) {
	Timer* timer = (Timer*)lua_touserdata(L, 1);
	
	ticks += timerElapsed(TIMER_ID);
	
	if (timer->state == timerStateRunning) {
		timer->state = timerStatePause;
		timer->ticksOnPause = ticks;
	}
	
	return 0;
}

static int timer_reset(lua_State *L) {
	Timer* timer = lua_touserdata(L, 1);
	// Reset the timer with an initial time (in ms)
	lua_Number initialTime = 0;
	if (lua_gettop(L) > 1) initialTime = luaL_checknumber(L, 2);
	
	timer->state = timerStatePause;
	timer->initialTicks = 0;
	timer->ticksOnPause = initialTime * TIMER_CLOCK / 1000;
	
	return 0;
}


/* Needed for the two modified OS functions that follow
** This is a raw copy from loslib.c
*/

#if !defined(LUA_STRFTIMEOPTIONS)

#if !defined(LUA_USE_POSIX)
#define LUA_STRFTIMEOPTIONS	{ "aAbBcdHIjmMpSUwWxXyYz%", "" }
#else
#define LUA_STRFTIMEOPTIONS \
	{ "aAbBcCdDeFgGhHIjmMnprRStTuUVwWxXyYzZ%", "" \
	  "", "E", "cCxXyY",  \
	  "O", "deHImMSuUVwWy" }
#endif

#endif

#if defined(LUA_USE_GMTIME_R)

#define l_gmtime(t,r)		gmtime_r(t,r)
#define l_localtime(t,r)	localtime_r(t,r)

#elif !defined(l_gmtime)

#define l_gmtime(t,r)		((void)r, gmtime(t))
#define l_localtime(t,r)  	((void)r, localtime(t))

#endif

static void setfield (lua_State *L, const char *key, int value) {
  lua_pushinteger(L, value);
  lua_setfield(L, -2, key);
}

static void setboolfield (lua_State *L, const char *key, int value) {
  if (value < 0)  /* undefined? */
    return;  /* does not set field */
  lua_pushboolean(L, value);
  lua_setfield(L, -2, key);
}

static int getboolfield (lua_State *L, const char *key) {
  int res;
  lua_getfield(L, -1, key);
  res = lua_isnil(L, -1) ? -1 : lua_toboolean(L, -1);
  lua_pop(L, 1);
  return res;
}

static int getfield (lua_State *L, const char *key, int d) {
  int res, isnum;
  lua_getfield(L, -1, key);
  res = (int)lua_tointegerx(L, -1, &isnum);
  if (!isnum) {
    if (d < 0)
      return luaL_error(L, "field " LUA_QS " missing in date table", key);
    res = d;
  }
  lua_pop(L, 1);
  return res;
}

static const char *checkoption (lua_State *L, const char *conv, char *buff) {
  static const char *const options[] = LUA_STRFTIMEOPTIONS;
  unsigned int i;
  for (i = 0; i < sizeof(options)/sizeof(options[0]); i += 2) {
    if (*conv != '\0' && strchr(options[i], *conv) != NULL) {
      buff[1] = *conv;
      if (*options[i + 1] == '\0') {  /* one-char conversion specifier? */
        buff[2] = '\0';  /* end buffer */
        return conv + 1;
      }
      else if (*(conv + 1) != '\0' &&
               strchr(options[i + 1], *(conv + 1)) != NULL) {
        buff[2] = *(conv + 1);  /* valid two-char conversion specifier */
        buff[3] = '\0';  /* end buffer */
        return conv + 2;
      }
    }
  }
  luaL_argerror(L, 1,
    lua_pushfstring(L, "invalid conversion specifier '%%%s'", conv));
  return conv;  /* to avoid warnings */
}

// Functions associated with the table Timer
static const luaL_Reg timerlib[] = {
    {"new", timer_new},
    {NULL, NULL}
};

// Methods of the Timer instances
static const luaL_Reg timerlib_methods[] = {
    {"getTime", timer_time},
    {"start", timer_start},
    {"stop", timer_stop},
    {"reset", timer_reset},
    {NULL, NULL}
};

/*
** Open infos library
*/
LUALIB_API int luaopen_timer(lua_State *L) {
    // Add shims for time3DS/date3DS (MicroLua 4.7.2 compatibility)
    lua_getglobal(L, LUA_OSLIBNAME);
    lua_getfield(L, -2, "time");
    lua_setfield(L, -2, "time3DS");
    lua_getfield(L, -2, "date");
    lua_setfield(L, -2, "date3DS");
    lua_pop(L, 1);

    // Create the metatable for the instances of Timer
    luaL_newmetatable(L, TIMER_META);
    lua_pushliteral(L, "__index");
    luaL_newlib(L, timerlib_methods);		// Create a table for the available methods
    lua_rawset(L, -3);						// It is then stored as the __index

    // Finally open the lib (basically register "Timer.new([time])")
    luaL_newlib(L, timerlib);

    return 1;
}

