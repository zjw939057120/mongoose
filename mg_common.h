#ifndef MG_COMMON_H
#define MG_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "third_party/mongoose/mongoose.h"
#include "third_party/cJSON/cJSON.h"
#include "third_party/cJSON/cJSON_Utils.h"

#define HTTP_ADDR "http://0.0.0.0:80"  // HTTP port
#define ROOT_DIR "web_root"
#define HTML_SUFFIX ".html"
#define MUSTACHE_SUFFIX ".html.mustache"

#define DEFAULT_HTML_PATH "web_root/page.html"

#define JSON_CONTENT_TYPE "Content-Type: application/json;charset=utf-8\r\n"
#define HTML_CONTENT_TYPE "Content-Type: text/html;charset=utf-8\r\n"

#ifdef __cplusplus
}
#endif
#endif
