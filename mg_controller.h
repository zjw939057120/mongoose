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

#define JSON_CONTENT_TYPE "Content-Type: application/json;charset=utf-8\r\n"
#define HTML_CONTENT_TYPE "Content-Type: text/html;charset=utf-8\r\n"

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
 * @param opts 选项指针
 */
void static_file_handle(struct mg_connection *c, struct mg_http_message *hm, const struct mg_http_serve_opts *opts);

/**
 * @brief 处理根URL请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 * @param opts 选项指针
 */
void get_root_url_process(struct mg_connection *c, struct mg_http_message *hm, const struct mg_http_serve_opts *opts);

/**
 * @brief 处理模型根请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 * @param opts 选项指针
 */
void get_model_root_process(struct mg_connection *c, struct mg_http_message *hm, const struct mg_http_serve_opts *opts);
#ifdef __cplusplus
}
#endif
#endif