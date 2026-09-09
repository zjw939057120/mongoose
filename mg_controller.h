#ifndef MG_CONTROLLER_H
#define MG_CONTROLLER_H

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

/**
 * @brief 处理HTTP消息
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 * @param opts 选项指针
 */
void mg_http_message_handle(struct mg_connection *c, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *opts);
                       
#ifdef __cplusplus
}
#endif
#endif