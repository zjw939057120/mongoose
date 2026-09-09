#ifndef MG_ROUTER_H
#define MG_ROUTER_H

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

bool router_handle(struct mg_connection *c, struct mg_http_message *hm,
                         const struct mg_http_serve_opts *opts);
                       
#ifdef __cplusplus
}
#endif
#endif