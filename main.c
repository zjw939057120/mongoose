// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
//
// UI example
// It implements the following endpoints:
//    /api/config/get - respond with current config
//    /api/config/set - POST a config change
//    any other URI serves static files from s_root_dir
// Data and results are JSON strings

#include "mongoose.h"
#include "mg_middleware.h"
#include "mg_model.h"
#include "mg_controller.h"
#include "mg_view.h"

static const char *s_http_addr = "http://0.0.0.0:80";  // HTTP port
static const char *s_root_dir = "web_root";

// Try to update a single configuration value
static void update_config(struct mg_str json, const char *path, char **value) {
  char *jval;
  if ((jval = mg_json_get_str(json, path)) != NULL) {
    mg_free(*value);
    *value = jval;
  }
}

/**
 * @brief HTTP服务选项
 * 
 */
struct mg_http_serve_opts opts;

/**
 * @brief 处理HTTP事件
 * 
 * @param c 连接指针
 * @param ev 事件类型
 * @param ev_data 事件数据指针
 */
static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN && c->is_listening) {
    // 连接打开时，设置根目录为 s_root_dir
    opts.root_dir = s_root_dir;
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    // 处理路由
    if (!router_handle(c, hm)) {
      // 未匹配到路由，处理静态文件
      static_file_handle(c, hm, &opts);
    }
  }
}

int main(void) {
  model_init(); // 初始化模型
  struct mg_mgr mgr;                            // Event manager
  mg_log_set(MG_LL_INFO);                       // Set to 3 to enable debug
  mg_mgr_init(&mgr);                            // Initialise event manager
  mg_http_listen(&mgr, s_http_addr, fn, NULL);  // Create HTTP listener
  for (;;) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
  mg_mgr_free(&mgr);
  model_deinit(); // 释放模型资源
  return 0;
}
