#include "mg_controller.h"
#include "mg_model.h"
#include "mg_view.h"

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
    mg_url_decode(hm->uri.buf, hm->uri.len, path, sizeof(path), 0);
    // 处理 HTML 文件
    const char *html_suffix = ".html";
    size_t suffix_len = strlen(html_suffix);
    size_t path_len = strlen(path);
    if (path_len >= suffix_len && memcmp(path + path_len - suffix_len, html_suffix, suffix_len) == 0) {
        if(!has_access_token(c, hm) && strcmp(path, "/login.html") != 0) {
            // 302 重定向到登录页
            mg_http_reply(c, 302, "Location: /login.html\r\n", "");
            return;
        }
      mg_http_serve_file(c, hm, "web_root/page.html", opts);
      return;
    }
    // 处理 Mustache 模板文件
    const char *mustache_suffix = ".html.mustache";
    suffix_len = strlen(mustache_suffix);
    path_len = strlen(path);
    if (path_len >= suffix_len && memcmp(path + path_len - suffix_len, mustache_suffix, suffix_len) == 0) {
      char temp[MG_PATH_MAX];
      mg_snprintf(temp, sizeof(temp), "%s/%s", opts->root_dir, path);
      // 读取 Mustache 模板文件
      struct mg_str content = mg_file_read(&mg_fs_posix, temp);
      
      if (content.buf != NULL) {

        // 根据路径选择模板标签
        // 示例： /system.html.mustache 时，选择 system 模板
        char *suffix_pos = strstr(path, mustache_suffix);
        if (suffix_pos != NULL) {
            *suffix_pos = '\0';
        }
        // 移除开头的/
        const char *node_name = path;
        if (node_name[0] == '/') {
            node_name++;
        }
        // 从模型根节点中获取对应节点
        cJSON *node = cJSON_GetObjectItem(get_model_root(), node_name);
        // 执行 Mustache 渲染
        char *rendered_html = render_mustache(node, content.buf);
        
        if (rendered_html != NULL) {
            // 将渲染后的 HTML 输出到页面
            mg_http_reply(c, 200, HTML_CONTENT_TYPE, "%s", rendered_html);
            free(rendered_html); // 必须释放渲染分配的内存
        } else {
            mg_http_reply(c, 500, "", "Template rendering failed\n");
        }
        
        mg_free((void *)content.buf); // 必须释放读取文件分配的内存
    } else {
        mg_http_reply(c, 404, "", "Template file not found\n");
    }
    return; // 处理完毕，直接返回
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
    char *json_str = cJSON_Print(get_model_root());
    response_json(c, json_str);    
    // 使用完毕后释放内存
    cJSON_free(json_str); 
}

