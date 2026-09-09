#ifndef MG_VIEW_H
#define MG_VIEW_H

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
 * @brief 获取 Mustache 模板中的变量值
 * 
 * @param node 模板节点
 * @param var_name 变量名
 * @return const char* 变量值或默认值
 */
const char* get_variable_value(cJSON *node, const char *var_name);

/**
 * @brief 渲染 Mustache 模板
 * 
 * @param node 模板节点
 * @param html_content Mustache 模板内容
 * @return char* 渲染后的 HTML 内容
 */
char *render_mustache(cJSON *node, const char *html_content);

#ifdef __cplusplus
}
#endif
#endif
