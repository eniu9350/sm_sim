#ifndef _SM_SIM_EV_HANDLER_
#define _SM_SIM_EV_HANDLER_

#include "ev_loop.h"
#include "ev.h"

/*------- server event handler--------------------------*/
void ev_handle_server_hb_req(ev_loop* el, ev* e);
void ev_handle_server_hb_resp(ev_loop* el, ev* e);
void ev_handle_server_bc_req(ev_loop* el, ev* e);
void ev_handle_server_check_hb(ev_loop* el, ev* e);
void ev_handle_server_srv_req(ev_loop* el, ev* e);

/*------- client event handler--------------------------*/
void ev_handle_client_power_on(ev_loop* el, ev* e);
void ev_handle_client_hb_req(ev_loop* el, ev* e);
void ev_handle_client_bc_req(ev_loop* el, ev* e);
void ev_handle_client_switching(ev_loop* el, ev* e);
void ev_handle_client_srv_req(ev_loop* el, ev* e);	//mmmmmmmmmmmmm: server srv req should be added indirectly through this event, and current chid should be set

void ev_handle_hbrsp(ev_loop* el,ev* e);
void ev_handle_bcreq(ev_loop* el,ev* e);
void ev_handle_bcrsp(ev_loop* el,ev* e);
void ev_handle_srvreq(ev_loop* el,ev* e);

#endif
