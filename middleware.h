#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

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
/**
 * @brief 获取 Mustache 模板中的变量值
 * 
 * @param item 模板项
 * @param var_name 变量名
 * @return const char* 变量值或默认值
 */
const char* get_variable_value(cJSON *item, const char *var_name);

/**
 * @brief 渲染 Mustache 模板
 * 
 * @param item 模板项
 * @param html_content Mustache 模板内容
 * @return char* 渲染后的 HTML 内容
 */
char *render_mustache(cJSON *item, const char *html_content);

/**
 * @brief 初始化中间件
 * 
 */
void middleware_init(void);
/**
 * @brief 释放中间件资源
 * 
 */
void middleware_deinit(void);

#ifdef __cplusplus
}
#endif
#endif