#include "middleware.h"

cJSON *tpl = NULL;
cJSON *tpl_index = NULL;
cJSON *tpl_system = NULL;

/**
 * @brief 初始化中间件
 * 
 */
void middleware_init(void) {
    // 初始化 Mustache 模板管理器的模板对象
    tpl = cJSON_CreateObject();
    // 初始化 index.html模板
    tpl_index = cJSON_CreateObject();
    cJSON_AddStringToObject(tpl_index, "appversion", __TIME__);
    cJSON_AddItemToObject(tpl, "index", tpl_index);
    // 初始化 system.html模板
    tpl_system = cJSON_CreateObject();
    cJSON_AddStringToObject(tpl_system, "V001", "[标签01]");
    cJSON_AddStringToObject(tpl_system, "V002", "[标签02]");
    cJSON_AddStringToObject(tpl_system, "V003", "[标签03]");
    cJSON_AddStringToObject(tpl_system, "V004", "[标签04]");
    cJSON_AddStringToObject(tpl_system, "V005", "[标签05]");
    cJSON_AddStringToObject(tpl_system, "V006", "[标签06]");
    cJSON_AddStringToObject(tpl_system, "V007", "[标签07]");
    cJSON_AddStringToObject(tpl_system, "V008", "[标签08]");
    cJSON_AddStringToObject(tpl_system, "V009", "[标签09]");
    cJSON_AddStringToObject(tpl_system, "V010", "[标签10]");
    cJSON_AddStringToObject(tpl_system, "V011", "[标签11]");
    cJSON_AddStringToObject(tpl_system, "V012", "[标签12]");
    cJSON_AddItemToObject(tpl, "system", tpl_system);
}

/**
 * @brief 释放中间件资源
 * 
 */
void middleware_deinit(void) {
    cJSON_Delete(tpl);
    tpl = NULL;
    cJSON_Delete(tpl_index);
    tpl_index = NULL;
    cJSON_Delete(tpl_system);
    tpl_system = NULL;
}

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
      // 1. 读取 Mustache 模板文件
      struct mg_str content = mg_file_read(&mg_fs_posix, temp);
      
      if (content.buf != NULL) {

        // 2. 根据路径选择模板标签
        // 示例： /system.html.mustache 时，选择 system 模板
        char *suffix_pos = strstr(path, mustache_suffix);
        if (suffix_pos != NULL) {
            *suffix_pos = '\0';
        }
        //移除开头的/
        const char *template_name = path;
        if (template_name[0] == '/') {
            template_name++;
        }
        cJSON *item = cJSON_GetObjectItem(tpl, template_name);
        // 2. 执行 Mustache 渲染
        char *rendered_html = render_mustache(item, content.buf);
        
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

/**
 * @brief 获取 Mustache 模板中的变量值
 * 
 * @param item 模板项
 * @param var_name 变量名
 * @return const char* 变量值或默认值
 */
const char* get_variable_value(cJSON *item, const char *var_name) {
    cJSON *var_item = cJSON_GetObjectItem(item, var_name);
    // 安全检查：判断节点是否存在且类型为字符串
    if (cJSON_IsString(var_item) && (var_item->valuestring != NULL)) {
        return var_item->valuestring;
    } else {
        return "[Unknown Variable]";
    }
}

/**
 * @brief 渲染 Mustache 模板
 * 
 * @param path 模板路径
 * @param html_content Mustache 模板内容
 * @return char* 渲染后的 HTML 内容
 */
char *render_mustache(cJSON *item, const char *html_content) {
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
        const char *value = get_variable_value(item, var_name);
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
