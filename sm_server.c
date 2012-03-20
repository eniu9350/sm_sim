#include "sm_server.h"

#include <stdlib.h>


sm_server* sm_server_create()
{
	sm_server* s;
	s = (sm_server*)malloc(sizeof(sm_server));
	s->ci = create_ch_alist();
	s->buf_hb_req = create_evdata_server_hb_req_alist();
	s->start = 0;
	return s;
}
