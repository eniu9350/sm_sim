#ifndef _SM_SIM_EV_HANDLER_
#define _SM_SIM_EV_HANDLER_

#include "ev_loop.h"
#include "ev.h"

void ev_handle_hbreq(ev_loop* el,ev* e);
void ev_handle_hbreq_client(ev_loop* el,ev* e);
void ev_handle_checkhb(ev_loop* el, ev* e);
void ev_handle_hbrsp(ev_loop* el,ev* e);
void ev_handle_bcreq(ev_loop* el,ev* e);
void ev_handle_bcrsp(ev_loop* el,ev* e);
void ev_handle_srvreq(ev_loop* el,ev* e);

#endif
