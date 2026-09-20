#ifndef MG_CONTROLLER_H
#define MG_CONTROLLER_H

#include "mg_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief HTTP服务选项
 * 
 */
extern struct mg_http_serve_opts opts;

/**
 * @brief 响应404错误
 * 
 * @param c 连接指针
 */
void response_not_found(struct mg_connection *c);

/**
 * @brief 响应JSON数据
 * 
 * @param c 连接指针
 * @param json_str JSON字符串指针
 */
void response_json(struct mg_connection *c, char *json_str);

/**
 * @brief 响应HTML数据
 * 
 * @param c 连接指针
 * @param html_str HTML字符串指针
 */
void response_html(struct mg_connection *c, char *html_str);
/**
 * @brief 处理静态文件请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void static_file_handle(struct mg_connection *c, struct mg_http_message *hm, struct mg_http_serve_opts *opts);

#ifdef __cplusplus
}
#endif
#endif