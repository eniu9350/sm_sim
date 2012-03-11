#include "sim_env.h"

sim_env se;

int main()
{
	sim_env_init(&se);

	ev_loop_loop(se.el);
}
