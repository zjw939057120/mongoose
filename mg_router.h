#ifndef MG_ROUTER_H
#define MG_ROUTER_H

#include "mg_common.h"

#ifdef __cplusplus
extern "C" {
#endif

bool router_handle(struct mg_connection *c, struct mg_http_message *hm);
                       
#ifdef __cplusplus
}
#endif
#endif