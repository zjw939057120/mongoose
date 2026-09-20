#ifndef MG_AUTHENTICATION_H
#define MG_AUTHENTICATION_H

#include "mg_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 处理登录请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void post_authentication_login_handle(struct mg_connection *c, struct mg_http_message *hm);
/**
 * @brief 处理注销请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void post_authentication_logout_handle(struct mg_connection *c, struct mg_http_message *hm);

/**
 * @brief 检查是否需要登录
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 * @return true 如果需要登录，否则返回false
 */
bool has_access_token(struct mg_connection *c, struct mg_http_message *hm);

#ifdef __cplusplus
}
#endif
#endif