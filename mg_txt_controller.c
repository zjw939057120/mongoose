#include "mg_txt_controller.h"
#include "mg_controller.h"
#include "mg_model.h"
#include "mg_view.h"

/**
 * @brief 处理系统页 HTML 模板请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_txt_html_handle(struct mg_connection *c, struct mg_http_message *hm) {
    // 获取参数修改页模型
    cJSON *model_txt = get_model_txt();
    // 从查询参数中获取 W、C、V、RET、RETW 字段值
    // 示例：?W=7&C=103498&V=YK-BA6401MKTV1&RET=%2Fsystem.html&RETW=5
    char param_value[64] = {0};
    mg_http_get_var(&hm->query, "W", param_value, sizeof(param_value));
    if (param_value[0] != '\0') {
        cJSON_ReplaceItemInObject(model_txt, "W", cJSON_CreateString(param_value));
    }
    mg_http_get_var(&hm->query, "C", param_value, sizeof(param_value));
    if (param_value[0] != '\0') {
        cJSON_ReplaceItemInObject(model_txt, "C", cJSON_CreateString(param_value));
    }
    mg_http_get_var(&hm->query, "V", param_value, sizeof(param_value));
    if (param_value[0] != '\0') {
        cJSON_ReplaceItemInObject(model_txt, "V", cJSON_CreateString(param_value));
    }
    mg_http_get_var(&hm->query, "RET", param_value, sizeof(param_value));
    if (param_value[0] != '\0') {
        cJSON_ReplaceItemInObject(model_txt, "RET", cJSON_CreateString(param_value));
    }
    mg_http_get_var(&hm->query, "RETW", param_value, sizeof(param_value));
    if (param_value[0] != '\0') {
        cJSON_ReplaceItemInObject(model_txt, "RETW", cJSON_CreateString(param_value));
    }
    mg_http_serve_file(c, hm, DEFAULT_HTML_PATH, &opts);
}

/**
 * @brief 处理参数修改页 Mustache 模板请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_txt_mustache_handle(struct mg_connection *c, struct mg_http_message *hm) {
    // 渲染 Mustache 模板
    render_node(c, MODEL_TXT);
}
