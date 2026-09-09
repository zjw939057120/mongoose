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
#define MODEL_ETHDISPLAY "ethdisplay"
#define MODEL_RS485 "rs485"
#define MODEL_BACFG "baccfg"
#define MODEL_DIPINS "dipins"
#define MODEL_DOPINS "dopins"
#define MODEL_UIPINS "uipins"
#define MODEL_DOINS "doins"

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
cJSON * get_model_index(void);
/**
 * @brief 获取数据模型系统节点
 * 
 * @return cJSON* 模型系统节点
 */
cJSON * get_model_system(void);
/**
 * @brief 获取数据模型以太网显示节点
 * 
 * @return cJSON* 模型以太网显示节点
 */
cJSON * get_model_ethdisplay(void);
/**
 * @brief 获取数据模型RS485节点
 * 
 * @return cJSON* 模型RS485节点
 */
cJSON * get_model_rs485(void);
/**
 * @brief 获取数据模型BACFG节点
 * 
 * @return cJSON* 模型BACFG节点
 */
cJSON * get_model_baccfg(void);
/**
 * @brief 获取数据模型DIPINS节点
 */
cJSON * get_model_dipins(void);
/**
 * @brief 获取数据模型DOPINS节点
 */
cJSON * get_model_dopins(void);
/**
 * @brief 获取数据模型UIPINS节点
 */
cJSON * get_model_uipins(void);
/**
 * @brief 获取数据模型DOINS节点
 */
cJSON * get_model_doins(void);



#ifdef __cplusplus
}
#endif
#endif
