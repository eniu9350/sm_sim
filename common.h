#ifndef _SM_SIM_COMMON_
#define _SM_SIM_COMMON_

#include "event.h"
#include "log.h"
//#include "server.h"

/*------- prop name--------------------------*/
#define PN_DEMUL_INTERVAL "demul.interval"
#define PN_TRAN_DELAY "tran.delay"

//mmm:temp
#define T_TRAN 10
#define T_STEP 1

#define VALUE_TYPE_INT 1
#define VALUE_TYPE_STRING 2


//struct ev_loop;
//struct evhandler_table;

/*------- prop container --------------------------*/
typedef union value	{
	int type;
	int n;
	char s[100];
}value;

typedef struct property	{
	char k[50];
	value v;
}property;

typedef struct properties	{
}properties;


/*------- context --------------------------*/
typedef struct context_global	{
	long now;
	struct ev_loop* el;
	struct evhandler_table* ehtable;
	server* server;
	//logging utils
}context_global;

#endif
