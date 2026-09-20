#include "mg_index_controller.h"
#include "mg_controller.h"
#include "mg_model.h"
#include "mg_view.h"

void get_home_handle(struct mg_connection *c, struct mg_http_message *hm) {
    // 重定向到首页
    mg_http_reply(c, 302, "Location: /index.html\r\n", "");
    return;
}

/**
 * @brief 处理首页 HTML 模板请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_index_html_handle(struct mg_connection *c, struct mg_http_message *hm) {
    // 获取首页模型
    cJSON *index = get_model_index();
    // 替换 appversion 字段为当前时间
    cJSON_ReplaceItemInObject(index, "appversion", cJSON_CreateString(__DATE__" "__TIME__));
    mg_http_serve_file(c, hm, DEFAULT_HTML_PATH, &opts);
}

/**
 * @brief 处理首页 Mustache 模板请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_index_mustache_handle(struct mg_connection *c, struct mg_http_message *hm) {
    // 渲染 Mustache 模板
    render_node(c, MODEL_INDEX);
}
