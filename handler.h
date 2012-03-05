#ifndef _SM_SIM_HANDLER_
#define _SM_SIM_HANDLER_

#include "event.h"

void handle_hbreq(ev* e);
void handle_hbrsp(ev* e);
void handle_bcreq(ev* e);
void handle_bcrsp(ev* e);
void handle_srvreq(ev* e);

#endif
