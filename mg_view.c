#include "mg_view.h"
#include "mg_common.h"
#include "mg_model.h"

/**
 * @brief 获取 Mustache 模板中的变量值
 * 
 * @param node 模板节点
 * @param var_name 变量名
 * @return const char* 变量值或默认值
 */
const char* get_variable_value(cJSON *node, const char *var_name) {
    cJSON *sub_node = cJSON_GetObjectItem(node, var_name);
    // 安全检查：判断节点是否存在且类型为字符串
    if (cJSON_IsString(sub_node) && (sub_node->valuestring != NULL)) {
        return sub_node->valuestring;
    } else {
        return "[Unknown Variable]";
    }
}

/**
 * @brief 渲染 Mustache 模板（不使用正则）
 *
 * 思路：循环查找 {{ 和 }}，将 {{ 和 }} 之间的内容作为变量名替换为实际值。
 *
 * @param node 模板节点
 * @param html_content Mustache 模板内容
 * @return char* 渲染后的 HTML 内容
 */
char *render_mustache(cJSON *node, const char *html_content) {
    if (html_content == NULL) return NULL;

    size_t content_len = strlen(html_content);
    // 预分配一个较大的缓冲区，足够容纳所有替换
    size_t buffer_size = content_len * 2 + 16;
    char *result = (char *)malloc(buffer_size);
    if (!result) return NULL;

    size_t result_len = 0;
    const char *cursor = html_content;

    while (*cursor) {
        // 1. 查找 "{{"
        const char *open_brace = strstr(cursor, "{{");
        if (open_brace == NULL) {
            // 没有更多 {{，将剩余内容全部追加
            size_t remaining_len = strlen(cursor);
            if (result_len + remaining_len >= buffer_size) break;
            memcpy(result + result_len, cursor, remaining_len);
            result_len += remaining_len;
            break;
        }

        // 2. 将 "{{" 之前的内容追加到结果
        size_t pre_len = open_brace - cursor;
        if (result_len + pre_len >= buffer_size) break;
        if (pre_len > 0) {
            memcpy(result + result_len, cursor, pre_len);
            result_len += pre_len;
        }

        // 3. 在 "{{" 之后查找 "}}"
        const char *close_brace = strstr(open_brace + 2, "}}");
        if (close_brace == NULL) {
            // 没有匹配的 "}}"，保留原文
            size_t tail_len = strlen(open_brace);
            if (result_len + tail_len >= buffer_size) break;
            memcpy(result + result_len, open_brace, tail_len);
            result_len += tail_len;
            break;
        }

        // 4. 提取变量名
        size_t var_len = close_brace - (open_brace + 2);
        if (var_len >= 64) var_len = 63;
        char var_name[64] = {0};
        memcpy(var_name, open_brace + 2, var_len);

        // 5. 获取真实变量值并追加到结果中
        const char *value = get_variable_value(node, var_name);
        size_t val_len = strlen(value);
        if (result_len + val_len >= buffer_size) break;
        memcpy(result + result_len, value, val_len);
        result_len += val_len;

        // 6. 移动游标到 "}}" 之后，继续下一次查找
        cursor = close_brace + 2;
    }

    result[result_len] = '\0'; // 确保字符串以 \0 结尾

    return result;
}

/**
 * @brief 渲染节点（根据模板名称）
 * 
 * @param c 连接指针
 * @param node 模板节点
 * @param template_name 模板名称
 */
void render_node_by_name(struct mg_connection *c, cJSON *node, const char *template_name) {
      char temp[MG_PATH_MAX];
      mg_snprintf(temp, sizeof(temp), "%s/%s%s", ROOT_DIR, template_name, MUSTACHE_SUFFIX);
      // 读取 Mustache 模板文件
      struct mg_str content = mg_file_read(&mg_fs_posix, temp);
      
      if (content.buf != NULL) {
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

/**
 * @brief 渲染节点（根据节点名称）
 * 
 * @param c 连接指针
 * @param node_name 节点名称
 */
void render_node(struct mg_connection *c, const char *node_name) {
    // 从模型根节点中获取对应节点
    cJSON *node = cJSON_GetObjectItem(get_model_root(), node_name);
    // 渲染节点
    render_node_by_name(c, node, node_name);
}
