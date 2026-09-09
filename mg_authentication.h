#ifndef MG_AUTHENTICATION_H
#define MG_AUTHENTICATION_H

#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mongoose.h"
#include "cJSON.h"
#include "cJSON_Utils.h"

#ifdef __cplusplus
extern "C" {
#endif

void post_authentication_login_process(struct mg_connection *c, struct mg_http_message *hm);
void post_authentication_logout_process(struct mg_connection *c, struct mg_http_message *hm);

#ifdef __cplusplus
}
#endif
#endif