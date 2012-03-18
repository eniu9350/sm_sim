#ifndef _SM_SIM_SIM_ENV_
#define _SM_SIM_SIM_ENV_

#include "sm_server.h"
#include "sm_client.h"


/* ----------- global settings ----------- */
#define CLIENT_COUNT 50
#define CHANNEL_COUNT 200
#define SWITCHING_COUNT 200



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
