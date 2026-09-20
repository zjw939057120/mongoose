#ifndef MG_INDEX_CONTROLLER_H
#define MG_INDEX_CONTROLLER_H

#include "mg_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 处理根路径请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_home_handle(struct mg_connection *c, struct mg_http_message *hm);

/**
 * @brief 处理首页 HTML 模板请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_index_html_handle(struct mg_connection *c, struct mg_http_message *hm);

/**
 * @brief 处理首页 Mustache 模板请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_index_mustache_handle(struct mg_connection *c, struct mg_http_message *hm);

#ifdef __cplusplus
}
#endif
#endif