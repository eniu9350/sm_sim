#ifndef _SM_SIM_SIM_ENV_
#define _SM_SIM_SIM_ENV_

#include "sm_server.h"
#include "sm_client.h"


/* ----------- global settings ----------- */
#define CLIENT_COUNT 50
#define CHANNEL_COUNT 200




typedef struct sim_env	{
	ev_loop* el;


	sm_server* server;
	
	sm_client** clients;
	int nclients;
	
}sim_env;

void sim_env_init(sim_env* se);

#endif
