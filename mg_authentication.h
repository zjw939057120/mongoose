#ifndef MG_AUTHENTICATION_H
#define MG_AUTHENTICATION_H

#include "mg_common.h"

#ifdef __cplusplus
extern "C" {
#endif

void post_authentication_login_process(struct mg_connection *c, struct mg_http_message *hm);
void post_authentication_logout_process(struct mg_connection *c, struct mg_http_message *hm);

#ifdef __cplusplus
}
#endif
#endif