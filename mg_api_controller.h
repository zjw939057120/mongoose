#ifndef MG_API_CONTROLLER_H
#define MG_API_CONTROLLER_H

#include "mg_common.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief 处理获取模型列表请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_api_model_handle(struct mg_connection *c, struct mg_http_message *hm);

#ifdef __cplusplus
}
#endif
#endif