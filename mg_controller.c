#include "mg_controller.h"
#include "cJSON.h"
#include "cJSON_Utils.h"
#include "mg_common.h"
#include "mg_model.h"
#include "mg_view.h"

struct mg_http_serve_opts opts = {.root_dir = ROOT_DIR};

 bool has_access_token(struct mg_connection *c, struct mg_http_message *hm) {
// 1. 获取完整的 Cookie 报头
struct mg_str *cookie_hdr = mg_http_get_header(hm, "Cookie");

char var_access_token[64] = {0};

if (cookie_hdr != NULL) {
    // 2. 从 Cookie 报头中提取 access_token 的值
    struct mg_str token = mg_http_get_header_var(*cookie_hdr, mg_str("access_token"));
    
    // 判断是否成功提取到值（mg_str 的 len > 0 表示有值）
    if (token.len > 0) {
        // 将 mg_str 安全地拷贝到你的缓冲区中
        snprintf(var_access_token, sizeof(var_access_token), "%.*s", (int)token.len, token.buf);
        return true;
    } else {
        return false;
    }
} else {
    return false;
}
    return strcmp(var_access_token, "") == 0;
 }

/**
 * @brief 响应404错误
 * 
 * @param c 连接指针
 */
void response_not_found(struct mg_connection *c) {
    mg_http_reply(c, 404, JSON_CONTENT_TYPE, "404 Not Found");
}

/**
 * @brief 响应JSON数据
 * 
 * @param c 连接指针
 * @param json_str JSON字符串指针
 */
void response_json(struct mg_connection *c, char *json_str) {
    mg_http_reply(c, 200, JSON_CONTENT_TYPE, json_str);    
}

/**
 * @brief 响应HTML数据
 * 
 * @param c 连接指针
 * @param html_str HTML字符串指针
 */
void response_html(struct mg_connection *c, char *html_str) {
    mg_http_reply(c, 200, HTML_CONTENT_TYPE, html_str);    
}

/**
 * @brief 处理静态文件请求
 * 
 * @param c 连接上下文
 * @param hm HTTP 消息上下文
 * @param opts HTTP 服务选项
 */
void static_file_handle(struct mg_connection *c, struct mg_http_message *hm, struct mg_http_serve_opts *opts) {
     // 解码 URL 路径
    char path[MG_PATH_MAX];
    mg_url_decode(hm->uri.buf, hm->uri.len, path, sizeof(path), 0);;
    // 处理 HTML 文件
    size_t suffix_len = strlen(HTML_SUFFIX);
    size_t path_len = strlen(path);
    if (path_len >= suffix_len && memcmp(path + path_len - suffix_len, HTML_SUFFIX, suffix_len) == 0) {
        if(!has_access_token(c, hm) && strcmp(path, "/login.html") != 0) {
            // 302 重定向到登录页
            mg_http_reply(c, 302, "Location: /login.html\r\n", "");
            return;
        }
      mg_http_serve_file(c, hm, DEFAULT_HTML_PATH, opts);
      return;
    }
    // 处理 Mustache 模板文件
    suffix_len = strlen(MUSTACHE_SUFFIX);
    path_len = strlen(path);
    if (path_len >= suffix_len && memcmp(path + path_len - suffix_len, MUSTACHE_SUFFIX, suffix_len) == 0) {
        // 根据路径选择模板标签
        // 示例： /system.html.mustache 时，选择 system 模板
        char *suffix_pos = strstr(path, MUSTACHE_SUFFIX);
        if (suffix_pos != NULL) {
            *suffix_pos = '\0';
        }
        // 移除开头的/
        const char *node_name = path;
        if (node_name[0] == '/') {
            node_name++;
        }
        render_node(c, node_name);
        return;
    }
    // 处理其他文件
    mg_http_serve_dir(c, hm, opts);
}

void get_home_handle(struct mg_connection *c, struct mg_http_message *hm) {
    // 重定向到首页
    mg_http_reply(c, 302, "Location: /index.html\r\n", "");
    return;
}

void api_get_model_handle(struct mg_connection *c, struct mg_http_message *hm) {
    cJSON *root = get_model_root();
    // 打印 JSON 字符串
    char *json_str = cJSON_Print(root);
    response_json(c, json_str);    
    // 使用完毕后释放内存
    cJSON_free(json_str); 
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
    // 从查询参数中获取 W、C、V、RET、RETW 字段值
    // 示例：?W=7&C=103498&V=YK-BA6401MKTV1&RET=%2Fsystem.html&RETW=5
    char param_value[64] = {0};
    mg_http_get_var(&hm->query, "W", param_value, sizeof(param_value));
    cJSON_ReplaceItemInObject(index, "W", cJSON_CreateString(param_value));
    mg_http_get_var(&hm->query, "C", param_value, sizeof(param_value));
    cJSON_ReplaceItemInObject(index, "C", cJSON_CreateString(param_value));
    mg_http_get_var(&hm->query, "V", param_value, sizeof(param_value));
    cJSON_ReplaceItemInObject(index, "V", cJSON_CreateString(param_value));
    mg_http_get_var(&hm->query, "RET", param_value, sizeof(param_value));
    cJSON_ReplaceItemInObject(index, "RET", cJSON_CreateString(param_value));
    mg_http_get_var(&hm->query, "RETW", param_value, sizeof(param_value));
    cJSON_ReplaceItemInObject(index, "RETW", cJSON_CreateString(param_value));
    mg_http_serve_file(c, hm, DEFAULT_HTML_PATH, &opts);
}

/**
 * @brief 处理首页 Mustache 模板请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_index_mustache_handle(struct mg_connection *c, struct mg_http_message *hm) {
    // 获取首页模型
    cJSON *index = get_model_index();
    // 渲染 Mustache 模板
    render_node(c, MODEL_INDEX);
}

/**
 * @brief 处理系统页 HTML 模板请求
 * 
 * @param c 连接指针
 * @param hm HTTP消息指针
 */
void get_system_html_handle(struct mg_connection *c, struct mg_http_message *hm) {
    // 获取系统页模型
    cJSON *system = get_model_system();
    // 替换 V001 字段为当前时间
    cJSON_ReplaceItemInObject(system, "V001", cJSON_CreateString(__DATE__" "__TIME__));
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
    cJSON *system = get_model_system();
    // 渲染 Mustache 模板
    render_node(c, MODEL_SYSTEM);
}
