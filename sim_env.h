#ifndef _SM_SIM_SIM_ENV_
#define _SM_SIM_SIM_ENV_

#include "sm_server.h"
#include "sm_client.h"


/* ----------- config names ----------- */
//see values below also
#define CFG_NAME_CHANNEL_COUNT "chcount"
/* --- clients --- */
#define CFG_NAME_CLIENT_COUNT "c_count"
#define CFG_NAME_CLIENT_ARRIVAL_DIST "c_arrival"
#define CFG_NAME_CLIENT_DURATION_DIST "c_duration"
#define CFG_NAME_CLIENT_SWITCHING_COUNT "c_swcount"
#define CFG_NAME_CLIENT_HEARTBEAT_INTERVAL "c_hbinterval"

/* --- server --- */
#define CFG_NAME_SERVER_BROADCAST_INTERVAL "s_bcinterval"

//Push, check and get (assume g variable of lua state is declared as "L");
//remember to pop it to gurantee repetitive use of this micro.

#define LUA_PCG_STRING(k, v)  \
	lua_getglobal(L, k);  \
if(!lua_isstring(L, -1))  { \
	printf("%s not string\n", k); \
} \
else  { \
	v = lua_tostring(L, -1);  \
}

#define LUA_PCG_INT(k, v) \
	lua_getglobal(L, k);  \
if(!lua_isnumber(L, -1))  { \
	printf("%s not number\n", k); \
} \
else  { \
	v = (int)lua_tonumber(L, -1); \
}

#define LUA_POP lua_pop(L, 1);

//#define CLIENT_COUNT 500



/* ----------- config values ----------- */
#define CFG_VALUE_CLIENT_ARRIVAL_DIST_SYNC "sync"
#define CFG_VALUE_CLIENT_ARRIVAL_DIST_RANDOM "random"

#define CFG_VALUE_CLIENT_DURATION_DIST_POISSON "poisson"
#define CFG_VALUE_CLIENT_DURATION_DIST_WTOIN "wtoin"	//dist specified in the "watching television over an ip network" paper


/* ----------- global settings ----------- */
//#define CHANNEL_COUNT 200
//#define CLIENT_COUNT 500
//#define SWITCHING_COUNT 100

//#define HEARTBEAT_INTERVAL 30
//#define BROADCAST_INTERVAL 60



/* ----------- type ----------- */
#define VALUE_TYPE_INT 1
#define VALUE_TYPE_LONG 2
#define VALUE_TYPE_CHARARRAY 3

#define VALUE_ID_INTERVAL_CLIENT_HEARTBEAT 1
#define VALUE_ID_INTERVAL_SERVER_BROADCAST 2

/* ----------- config ----------- */
#define CONFIG_TYPE_INFO_SIZE 1000

/* ----------- stat ----------- */
#define STAT_SIZE 1000

#define STAT_ID_SERVER_SRV_REQ_COUNT 1
#define STAT_ID_FREQ_USAGE_TIME_SERIES 2

typedef union value	{
	int i;
	long l;
	char s[100];
}value;

typedef struct value_type	{
	int id;
	int type;	//1.int; 2.long; 3.char[];
}value_type;


typedef struct sim_config	{
	value val[1000];
	value_type types[CONFIG_TYPE_INFO_SIZE];
}sim_config;

typedef struct sim_stats	{
	long val[STAT_SIZE];
}sim_stats;

/* ----------- sim env ----------- */
typedef struct sim_env	{
	ev_loop* el;


	sm_server* server;

	sm_client** clients;
	int nclients;

	sim_config* config;
	sim_stats* stat;

}sim_env;

/* ------ sim env ops -----------*/
void sim_env_init();

/* ------ client info related ops -----------*/
sm_client* sim_env_get_client_by_uid(int uid);

/* ------ config info related ops -----------*/
void* sim_env_get_config(int configid);
int sim_env_set_config(int configid, void* v);

/* ------ stat related ops -----------*/
void	sim_env_stat_init();
int sim_env_stat_set(int statid, long v);
long sim_env_stat_get(int statid);
int sim_env_stat_inc(int statid);

#endif
