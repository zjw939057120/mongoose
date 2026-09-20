#include "mg_model.h"

cJSON *model_root = NULL;
cJSON *model_global = NULL;
cJSON *model_index = NULL;
cJSON *model_system = NULL;
cJSON *model_ethdisplay = NULL;
cJSON *model_rs485 = NULL;
cJSON *model_baccfg = NULL;
cJSON *model_dipins = NULL;
cJSON *model_dopins = NULL;
cJSON *model_uipins = NULL;
cJSON *model_uopins = NULL;
cJSON *model_txt = NULL;

/**
 * @brief 初始化数据模型根节点资源
 * 
 */
void model_init(void) {
    // 初始化数据模型根节点
    model_root = cJSON_CreateObject();
    //插入MODEL_GLOBAL节点
    model_global = cJSON_CreateObject();
    cJSON_AddStringToObject(model_global, "user", "admin");
    cJSON_AddStringToObject(model_global, "pwd", "5678");
    cJSON_AddItemToObject(model_root, MODEL_GLOBAL, model_global);
    //插入MODEL_INDEX节点
    model_index = cJSON_CreateObject();
    cJSON_AddStringToObject(model_index, "appversion","");
    cJSON_AddItemToObject(model_root, MODEL_INDEX, model_index);
    //插入MODEL_SYSTEM节点
    model_system = cJSON_CreateObject();
    cJSON_AddStringToObject(model_system, "V001", "[标签01]");
    cJSON_AddStringToObject(model_system, "V002", "[标签02]");
    cJSON_AddStringToObject(model_system, "V003", "[标签03]");
    cJSON_AddStringToObject(model_system, "V004", "[标签04]");
    cJSON_AddStringToObject(model_system, "V005", "[标签05]");
    cJSON_AddItemToObject(model_root, MODEL_SYSTEM, model_system);
    //插入MODEL_ETHDISPLAY节点
    model_ethdisplay = cJSON_CreateObject();
    cJSON_AddStringToObject(model_ethdisplay, "V001", "[标签01]");
    cJSON_AddStringToObject(model_ethdisplay, "V002", "[标签02]");
    cJSON_AddStringToObject(model_ethdisplay, "V003", "[标签03]");
    cJSON_AddStringToObject(model_ethdisplay, "V004", "[标签04]");
    cJSON_AddStringToObject(model_ethdisplay, "V005", "[标签05]");
    cJSON_AddStringToObject(model_ethdisplay, "V004", "[标签04]");
    //插入MODEL_RS485节点
    model_rs485 = cJSON_CreateObject();
    cJSON_AddStringToObject(model_rs485, "V001", "[标签01]");
    cJSON_AddStringToObject(model_rs485, "V002", "[标签02]");
    cJSON_AddStringToObject(model_rs485, "V003", "[标签03]");
    cJSON_AddStringToObject(model_rs485, "V004", "[标签04]");
    cJSON_AddStringToObject(model_rs485, "V005", "[标签05]");
    cJSON_AddItemToObject(model_root, MODEL_RS485, model_rs485);
    //插入MODEL_BACCFG节点
    model_baccfg = cJSON_CreateObject();
    cJSON_AddStringToObject(model_baccfg, "V001", "[标签01]");
    cJSON_AddStringToObject(model_baccfg, "V002", "[标签02]");
    cJSON_AddStringToObject(model_baccfg, "V003", "[标签03]");
    cJSON_AddStringToObject(model_baccfg, "V004", "[标签04]");
    cJSON_AddStringToObject(model_baccfg, "V005", "[标签05]");
    cJSON_AddItemToObject(model_root, MODEL_BACCFG, model_baccfg);
    //插入MODEL_DIPINS节点
    model_dipins = cJSON_CreateObject();
    cJSON_AddStringToObject(model_dipins, "V001", "[标签01]");
    cJSON_AddStringToObject(model_dipins, "V002", "[标签02]");
    cJSON_AddStringToObject(model_dipins, "V003", "[标签03]");
    cJSON_AddStringToObject(model_dipins, "V004", "[标签04]");
    cJSON_AddItemToObject(model_root, MODEL_DIPINS, model_dipins);
    cJSON_AddStringToObject(model_dipins, "V002", "[标签02]");
    cJSON_AddStringToObject(model_dipins, "V003", "[标签03]");
    cJSON_AddStringToObject(model_dipins, "V004", "[标签04]");
    cJSON_AddStringToObject(model_dipins, "V005", "[标签05]");
    //插入MODEL_DOPINS节点
    model_dopins = cJSON_CreateObject();
    cJSON_AddStringToObject(model_dopins, "V001", "[标签01]");
    cJSON_AddStringToObject(model_dopins, "V002", "[标签02]");
    cJSON_AddStringToObject(model_dopins, "V003", "[标签03]");
    cJSON_AddStringToObject(model_dopins, "V004", "[标签04]");
    cJSON_AddStringToObject(model_dopins, "V005", "[标签05]");
    cJSON_AddItemToObject(model_root, MODEL_DOPINS, model_dopins);
    //插入MODEL_UIPINS节点
    model_uipins = cJSON_CreateObject();
    cJSON_AddStringToObject(model_uipins, "V001", "[标签01]");
    cJSON_AddStringToObject(model_uipins, "V002", "[标签02]");
    cJSON_AddStringToObject(model_uipins, "V003", "[标签03]");
    cJSON_AddStringToObject(model_uipins, "V004", "[标签04]");
    cJSON_AddStringToObject(model_uipins, "V005", "[标签05]");
    cJSON_AddItemToObject(model_root, MODEL_UIPINS, model_uipins);
    //插入MODEL_UOPINS节点
    model_uopins = cJSON_CreateObject();
    cJSON_AddStringToObject(model_uopins, "V001", "[标签01]");
    cJSON_AddStringToObject(model_uopins, "V002", "[标签02]");
    cJSON_AddStringToObject(model_uopins, "V003", "[标签03]");
    cJSON_AddStringToObject(model_uopins, "V004", "[标签04]");
    cJSON_AddStringToObject(model_uopins, "V005", "[标签05]");
    cJSON_AddItemToObject(model_root, MODEL_UOPINS, model_uopins);
    //插入MODEL_TXT节点
    model_txt = cJSON_CreateObject();
    cJSON_AddStringToObject(model_txt, "V001", "[标签01]");
    cJSON_AddStringToObject(model_txt, "V002", "[标签02]");
    cJSON_AddStringToObject(model_txt, "V003", "[标签03]");
    cJSON_AddStringToObject(model_txt, "V004", "[标签04]");
    cJSON_AddStringToObject(model_txt, "V005", "[标签05]");
    cJSON_AddStringToObject(model_txt, "W","");
    cJSON_AddStringToObject(model_txt, "C","");
    cJSON_AddStringToObject(model_txt, "V","");
    cJSON_AddStringToObject(model_txt, "RET","");
    cJSON_AddStringToObject(model_txt, "RETW","");
    cJSON_AddItemToObject(model_root, MODEL_TXT, model_txt);
}

cJSON * get_model_root(void) {
    return model_root;
}
/**
 * @brief 获取数据模型全局节点
 * 
 * @return cJSON* 模型全局节点
 */
cJSON * get_model_global(void) {
    return model_global;
}
cJSON * get_model_index(void) {
    return model_index;
}
cJSON * get_model_system(void) {
    return model_system;
}
cJSON * get_model_ethdisplay(void) {
    return model_ethdisplay;
}
cJSON * get_model_rs485(void) {
    return model_rs485;
}
cJSON * get_model_baccfg(void) {
    return model_baccfg;
}
cJSON * get_model_dipins(void) {
    return model_dipins;
}
cJSON * get_model_dopins(void) {
    return model_dopins;
}
cJSON * get_model_uipins(void) {
    return model_uipins;
}
cJSON * get_model_uopins(void) {
    return model_uopins;
}
cJSON * get_model_txt(void) {
    return model_txt;
}
