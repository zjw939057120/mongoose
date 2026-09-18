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
#include <pthread.h>
#include "mg_common.h"
#include "mg_router.h"
#include "mg_model.h"
#include "mg_controller.h"

/**
 * @brief HTTP服务选项
 * 
 */

/**
 * @brief 处理HTTP事件
 * 
 * @param c 连接指针
 * @param ev 事件类型
 * @param ev_data 事件数据指针
 */
static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN && c->is_listening) {
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    // 处理路由
    if (!router_handle(c, hm)) {
      // 未匹配到路由，处理静态文件
      static_file_handle(c, hm, &opts);
    }
  }
}

void *web_server_thread_func(void *arg) {
  model_init(); // 初始化模型
  struct mg_mgr mgr;                            // Event manager
  mg_log_set(MG_LL_INFO);                       // Set to 3 to enable debug
  mg_mgr_init(&mgr);                            // Initialise event manager
  mg_http_listen(&mgr, HTTP_ADDR, fn, NULL);  // Create HTTP listener
  for (;;) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
  mg_mgr_free(&mgr);
  model_deinit(); // 释放模型资源
}

int main(void) {
  pthread_t thread;
  // 创建线程
  pthread_create(&thread, NULL, web_server_thread_func, NULL);
  // 等待线程结束
  pthread_join(thread, NULL);
  // 主线程继续执行
  while (1) {
    sleep(60);
  }
  return 0;
}
