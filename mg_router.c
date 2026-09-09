#include "mg_router.h"
#include "mg_controller.h"

typedef struct {
    char *path; // 路径模式
    char *method; // 请求方法
    void (*handler)(struct mg_connection *c, struct mg_http_message *hm,
                    const struct mg_http_serve_opts *opts);
} router_t;

// 使用结构体数组而不是指针数组
// 增加一个全 NULL 的结束标志，方便后续用 for 循环遍历
router_t router_root[] = {
    {"/", "GET", get_root_url_process},
    {"/api/model", "GET", get_model_root_process},
    {NULL, NULL, NULL}  // 结束标志
};

bool router_handle(struct mg_connection *c, struct mg_http_message *hm,
                   const struct mg_http_serve_opts *opts) {
    // 通过遍历数组来匹配路由
    for (int i = 0; router_root[i].path != NULL; i++) {
        // 1. 匹配路径
        if (mg_match(hm->uri, mg_str(router_root[i].path), NULL)) {
            // 2. 匹配请求方法
            if (mg_strcmp(hm->method, mg_str(router_root[i].method)) == 0) {
                // 3. 调用对应的处理函数（注意：这里用 '.' 而不是 '->'）
                router_root[i].handler(c, hm, opts);
                return true; // 匹配成功并执行后，直接返回 true
            }
        }
    }
    return false;
}
