#include "mongoose.h"
#include "middleware.h"

void mg_http_serve_dir_ext(struct mg_connection *c, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *opts) {
    char path[MG_PATH_MAX];
    int flags = mg_url_decode(hm->uri.buf, hm->uri.len, 
                            path, sizeof(path), 0);
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
      // 1. 读取 Mustache 模板文件
      struct mg_str content = mg_file_read(&mg_fs_posix, temp);
      
      if (content.buf != NULL) {
        // 2. 执行 Mustache 渲染
        char *rendered_html = render_mustache(content.buf);
        
        if (rendered_html != NULL) {
            // 3. 将渲染后的 HTML 输出到页面
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

// 模拟一个获取真实变量的函数（实际开发中可替换为查数据库或读配置文件）
const char* get_variable_value(const char *var_name) {
    if (strcmp(var_name, "title") == 0) return "Welcome to Mongoose";
    if (strcmp(var_name, "user") == 0) return "Admin";
    return "[Unknown Variable]";
}

// 核心替换函数：查找 {{变量名}} 并替换为真实值
char *render_mustache(const char *html_content) {
    regex_t regex;
    regmatch_t pmatch[2]; // 0: 匹配整个 {{var}}, 1: 匹配括号内的 var
    const char *pattern = "\\{\\{([a-zA-Z0-9_]+)\\}\\}"; // 正则匹配 {{变量名}}
    
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        return NULL; // 正则编译失败
    }

    size_t content_len = strlen(html_content);
    size_t buffer_size = content_len * 2; // 预分配一个较大的缓冲区
    char *result = (char *)malloc(buffer_size);
    if (!result) return NULL;
    
    size_t result_len = 0;
    const char *cursor = html_content;

    // 循环查找并替换
    while (regexec(&regex, cursor, 2, pmatch, 0) == 0) {
        // 1. 将匹配到 {{...}} 之前的内容复制到结果缓冲区
        size_t pre_len = pmatch[0].rm_so;
        if (result_len + pre_len >= buffer_size) break; // 防止缓冲区溢出
        memcpy(result + result_len, cursor, pre_len);
        result_len += pre_len;

        // 2. 提取变量名（pmatch[1] 捕获了括号内的内容）
        size_t var_len = pmatch[1].rm_eo - pmatch[1].rm_so;
        char var_name[64] = {0};
        strncpy(var_name, cursor + pmatch[1].rm_so, var_len);

        // 3. 获取真实变量值并追加到结果中
        const char *value = get_variable_value(var_name);
        size_t val_len = strlen(value);
        if (result_len + val_len >= buffer_size) break;
        memcpy(result + result_len, value, val_len);
        result_len += val_len;

        // 4. 移动游标到 {{...}} 之后，继续下一次查找
        cursor += pmatch[0].rm_eo;
    }

    // 5. 将剩余未匹配的 HTML 内容追加到结果中
    size_t remaining_len = strlen(cursor);
    if (result_len + remaining_len < buffer_size) {
        memcpy(result + result_len, cursor, remaining_len);
        result_len += remaining_len;
    }
    
    result[result_len] = '\0'; // 确保字符串以 \0 结尾
    regfree(&regex); // 释放正则表达式资源
    
    return result;
}
