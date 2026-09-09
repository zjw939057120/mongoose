#ifndef MG_MODEL_H
#define MG_MODEL_H

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

// 模型模板名称
#define MODEL_ROOT "root"
#define MODEL_INDEX "index"
#define MODEL_SYSTEM "system"
#define MODEL_CONFIG "config"

/**
 * @brief 初始化数据模型根节点资源
 * 
 */
void model_init(void);
/**
 * @brief 释放数据模型根节点资源
 * 
 */ 
void model_deinit(void);

/**
 * @brief 获取数据模型根节点
 * 
 * @return cJSON* 模型根节点
 */
cJSON * get_model_root(void);

/**
 * @brief 获取数据模型索引节点
 * 
 * @return cJSON* 模型索引节点
 */
cJSON * get_model_system(void);

/**
 * @brief 获取数据模型配置节点
 * 
 * @return cJSON* 模型配置节点
 */
cJSON * get_model_config(void);

#ifdef __cplusplus
}
#endif
#endif
