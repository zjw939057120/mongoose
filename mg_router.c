#include "mg_router.h"
#include "mg_controller.h"
#include "mg_authentication.h"

typedef struct {
    char *path; // 路径模式
    char *method; // 请求方法
    void (*handler)(struct mg_connection *c, struct mg_http_message *hm);
} router_t;

// 使用结构体数组而不是指针数组
// 增加一个全 NULL 的结束标志，方便后续用 for 循环遍历
static router_t router[] = {
    {"/", "GET", get_home_handle},// 根路径
    {"/sys/api/login", "POST", post_authentication_login_process},// 登录 POST 路径
    {"/sys/api/logout", "POST", post_authentication_logout_process},// 退出登录 POST 路径
    {"/api/model", "GET", api_get_model_handle}, // 获取模型列表
    {NULL, NULL, NULL}  // 结束标志
};

bool router_handle(struct mg_connection *c, struct mg_http_message *hm) {
    // 通过遍历数组来匹配路由
    for (int i = 0; router[i].path != NULL; i++) {
        // 1. 匹配路径
        if (mg_match(hm->uri, mg_str(router[i].path), NULL)) {
            // 2. 匹配请求方法
            if (mg_strcmp(hm->method, mg_str(router[i].method)) == 0) {
                // 3. 调用对应的处理函数（注意：这里用 '.' 而不是 '->'）
                if(!has_access_token(c, hm) && strcmp(router[i].path, "/sys/api/login") != 0) {
                    // 302 重定向到登录页
                    mg_http_reply(c, 302, "Location: /login.html\r\n", "");
                    return true;
                }
                router[i].handler(c, hm);
                return true; // 匹配成功并执行后，直接返回 true
            }
        }
    }
    return false;
}
