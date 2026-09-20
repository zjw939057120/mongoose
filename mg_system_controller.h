#ifndef MG_SYSTEM_CONTROLLER_H
#define MG_SYSTEM_CONTROLLER_H

#include "mg_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 处理系统页 HTML 模板请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_system_html_handle(struct mg_connection *c, struct mg_http_message *hm);
/**
 * @brief 处理系统页 Mustache 模板请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_system_mustache_handle(struct mg_connection *c, struct mg_http_message *hm);

#ifdef __cplusplus
}
#endif
#endif