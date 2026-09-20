#include "mg_api_controller.h"
#include "mg_controller.h"
#include "mg_model.h"
#include "mg_view.h"


void get_api_model_handle(struct mg_connection *c, struct mg_http_message *hm) {
    cJSON *model_root = get_model_root();
    // 打印 JSON 字符串
    char *json_str = cJSON_Print(model_root);
    response_json(c, json_str);    
    // 使用完毕后释放内存
    cJSON_free(json_str); 
}