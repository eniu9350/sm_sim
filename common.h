#ifndef _SM_SIM_COMMON_
#define _SM_SIM_COMMON_

#include "event.h"
#include "log.h"

/*------- prop name--------------------------*/
#define PN_DEMUL_INTERVAL "demul.interval"
#define PN_TRAN_DELAY "tran.delay"

#define VALUE_TYPE_INT 1
#define VALUE_TYPE_STRING 2


struct ev_loop;

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
	//logging utils
}context_global;

#endif
