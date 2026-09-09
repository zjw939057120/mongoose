#include "mg_model.h"

cJSON *model_root = NULL;
cJSON *model_index = NULL;
cJSON *model_system = NULL;
cJSON *model_config = NULL;

/**
 * @brief 初始化数据模型根节点资源
 * 
 */
void model_init(void) {
    // 初始化数据模型根节点
    model_root = cJSON_CreateObject();
    //插入MODEL_INDEX节点
    model_index = cJSON_CreateObject();
    cJSON_AddStringToObject(model_index, "appversion", __TIME__);
    cJSON_AddItemToObject(model_root, MODEL_INDEX, model_index);
    //插入MODEL_SYSTEM节点
    model_system = cJSON_CreateObject();
    cJSON_AddStringToObject(model_system, "V001", "[标签01]");
    cJSON_AddStringToObject(model_system, "V002", "[标签02]");
    cJSON_AddStringToObject(model_system, "V003", "[标签03]");
    cJSON_AddStringToObject(model_system, "V004", "[标签04]");
    cJSON_AddStringToObject(model_system, "V005", "[标签05]");
    cJSON_AddStringToObject(model_system, "V006", "[标签06]");
    cJSON_AddStringToObject(model_system, "V007", "[标签07]");
    cJSON_AddStringToObject(model_system, "V008", "[标签08]");
    cJSON_AddStringToObject(model_system, "V009", "[标签09]");
    cJSON_AddStringToObject(model_system, "V010", "[标签10]");
    cJSON_AddStringToObject(model_system, "V011", "[标签11]");
    cJSON_AddStringToObject(model_system, "V012", "[标签12]");
    cJSON_AddItemToObject(model_root, MODEL_SYSTEM, model_system);
}

/**
 * @brief 释放数据模型根节点资源
 * 
 */
void model_deinit(void) {
    cJSON_Delete(model_root);
    model_root = NULL;
}

cJSON * get_model_root(void) {
    return model_root;
}

cJSON * get_model_index(void) {
    return model_index;
}
cJSON * get_model_system(void) {
    return model_system;
}
cJSON * get_model_config(void) {
    return model_config;
}
