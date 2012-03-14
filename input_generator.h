#ifndef _SM_SIM_INPUT_GENERATOR_
#define _SM_SIM_INPUT_GENERATOR_

#include "sm_client.h"

//generate 53 duration freq
typedef struct duration	{
	int t; //lasting time
	double f; //frequency
	int fint;
}duration;



//n: client counts
sm_client* generate_input_clients_simple_1(int n);

#endif
