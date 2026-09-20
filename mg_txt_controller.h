#ifndef MG_TXT_CONTROLLER_H
#define MG_TXT_CONTROLLER_H

#include "mg_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 处理参数修改页 HTML 模板请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_txt_html_handle(struct mg_connection *c, struct mg_http_message *hm);
/**
 * @brief 处理参数修改页 Mustache 模板请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_txt_mustache_handle(struct mg_connection *c, struct mg_http_message *hm);

#ifdef __cplusplus
}
#endif
#endif