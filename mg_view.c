#include "mg_view.h"
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
 * @brief 渲染 Mustache 模板
 * 
 * @param node 模板节点
 * @param html_content Mustache 模板内容
 * @return char* 渲染后的 HTML 内容
 */
char *render_mustache(cJSON *node, const char *html_content) {
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
        const char *value = get_variable_value(node, var_name);
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
