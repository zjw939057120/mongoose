#include "mg_authentication.h"
#include "mg_controller.h"
#include "mg_model.h"
#include "mg_view.h"


/**
 * @brief 登录会话
 * 
 * @param c 连接指针
 * @param user 用户名
 * @param pwd 密码
 */
void  authentication_login_session(struct mg_connection *c,const char *user,const char *pwd) {
    // 处理登录逻辑
  char cookie[256];
  mg_snprintf(cookie, sizeof(cookie),
            "Set-Cookie: access_token=%s_%s; Path=/; "
            "%sHttpOnly; SameSite=Lax; Max-Age=%d\r\n"
            "Location: /index.html\r\n",
            user, pwd, c->is_tls ? "Secure; " : "", 3600 * 24);
            mg_http_reply(c, 302, cookie, "ok\n");
}

/**
 * @brief 退出登录会话
 * 
 * @param c 连接指针
 */
void  authentication_logout_session(struct mg_connection *c) {
  char cookie[256];
  mg_snprintf(cookie, sizeof(cookie),
              "Set-Cookie: access_token=; Path=/; "
              "Expires=Thu, 01 Jan 1970 00:00:00 UTC; "
              "%sHttpOnly; Max-Age=0; \r\n"
              "Location: /login.html\r\n",
              c->is_tls ? "Secure; " : "");
  mg_http_reply(c, 302, cookie, "ok\n");
}

void post_authentication_login_process(struct mg_connection *c, struct mg_http_message *hm) {
    //认证登录
    char post_username[64] = {0};
    char post_password[64] = {0};
    // 获取表单参数
    mg_http_get_var(&hm->body, "username", post_username, sizeof(post_username));
    mg_http_get_var(&hm->body, "password", post_password, sizeof(post_password));
    // 从模型中获取用户名和密码
    cJSON *global = get_model_global();
    char *var_user = cJSON_GetObjectItem(global, "user")->valuestring;
    char *var_pwd = cJSON_GetObjectItem(global, "pwd")->valuestring;

    if (strcmp(post_username, var_user) != 0 || strcmp(post_password, var_pwd) != 0) {
      printf("%s: username: %s, password: %s fail!\n", __func__, post_username, post_password );
      authentication_logout_session(c);
      return;
    }
    printf("%s: username: %s, password: %s success!\n", __func__, post_username, post_password );
    authentication_login_session(c,var_user,var_pwd);
}

void post_authentication_logout_process(struct mg_connection *c, struct mg_http_message *hm) {
    //认证登录退出
    authentication_logout_session(c);
}

