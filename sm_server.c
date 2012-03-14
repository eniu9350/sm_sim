#include "sm_server.h"

#include <stdlib.h>


sm_server* sm_server_create()
{
	sm_server* s;
	s = (sm_server*)malloc(sizeof(sm_server));
	s->ci = ch_info_create();
	s->buf_hb_req = evdata_server_hb_req_list_create();
	return s;
}
