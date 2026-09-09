#include "mg_controller.h"
#include "mg_model.h"
#include "mg_view.h"


/**
 * @brief 处理 HTTP 请求
 * 
 * @param c 连接上下文
 * @param hm HTTP 消息上下文
 * @param opts HTTP 服务选项
 */
void mg_http_message_handle(struct mg_connection *c, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *opts) {
     // 解码 URL 路径
    char path[MG_PATH_MAX];
    mg_url_decode(hm->uri.buf, hm->uri.len, path, sizeof(path), 0);
    // 处理根路径
    if(mg_casecmp(path, "/") == 0) {
        mg_http_reply(c, 302, "Location: /index.html\r\n", "");
        return;
    }
    // 处理 HTML 文件
    const char *html_suffix = ".html";
    size_t suffix_len = strlen(html_suffix);
    size_t path_len = strlen(path);
    if (path_len >= suffix_len && memcmp(path + path_len - suffix_len, html_suffix, suffix_len) == 0) {
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
        cJSON *item = cJSON_GetObjectItem(get_model_root(), node_name);
        // 执行 Mustache 渲染
        char *rendered_html = render_mustache(item, content.buf);
        
        if (rendered_html != NULL) {
            // 将渲染后的 HTML 输出到页面
            mg_http_reply(c, 200, "Content-Type: text/html\r\n", "%s", rendered_html);
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

