#include "mg_controller.h"
#include "mg_authentication.h"
#include "mg_model.h"
#include "mg_view.h"

struct mg_http_serve_opts opts = {.root_dir = ROOT_DIR};

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

