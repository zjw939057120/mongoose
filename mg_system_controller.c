#include "mg_system_controller.h"
#include "mg_controller.h"
#include "mg_model.h"
#include "mg_view.h"

/**
 * @brief 处理系统页 HTML 模板请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_system_html_handle(struct mg_connection *c, struct mg_http_message *hm) {
    // 获取系统页模型
    cJSON *model_system = get_model_system();
    // 替换 V001 字段为当前时间
    cJSON_ReplaceItemInObject(model_system, "V001", cJSON_CreateString(__DATE__" "__TIME__));
    mg_http_serve_file(c, hm, DEFAULT_HTML_PATH, &opts);
}

/**
 * @brief 处理系统页 Mustache 模板请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_system_mustache_handle(struct mg_connection *c, struct mg_http_message *hm) {
    // 获取系统页模型
    // 渲染 Mustache 模板
    render_node(c, MODEL_SYSTEM);
}
