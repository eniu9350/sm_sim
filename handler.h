#ifndef _SM_SIM_HANDLER_
#define _SM_SIM_HANDLER_

#include "event.h"

void handle_hbreq(ev_loop* el,ev* e);
void handle_hbreq_client(ev_loop* el,ev* e);
void handle_checkhb(ev_loop* el, ev* e);
void handle_hbrsp(ev_loop* el,ev* e);
void handle_bcreq(ev_loop* el,ev* e);
void handle_bcrsp(ev_loop* el,ev* e);
void handle_srvreq(ev_loop* el,ev* e);

#endif
