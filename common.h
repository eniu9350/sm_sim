#ifndef _SM_SIM_COMMON_
#define _SM_SIM_COMMON_

//#include "event.h"
//#include "log.h"
//#include "server.h"


/*------- debug --------------------------*/
#ifdef DEBUG
#define LOG(args) printf args
#else
#define LOG(args) ((void) 0)
#endif /* DEBUG */

/*------- lua --------------------------*/
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/*------- prop name--------------------------*/
#define PN_DEMUL_INTERVAL "demul.interval"
#define PN_TRAN_DELAY "tran.delay"

//mmm:temp
#define T_TRAN 10
#define T_STEP 1

#define VALUE_TYPE_INT 1
#define VALUE_TYPE_STRING 2


struct ev_loop;
struct evhandler_table;

/*------- prop container --------------------------*/
/*
typedef struct property	{
	char k[50];
	val v;
}property;

typedef struct properties	{
}properties;
*/

#endif
