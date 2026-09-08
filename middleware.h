#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void mg_http_serve_dir_ext(struct mg_connection *c, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *opts);

const char* get_variable_value(const char *var_name);

char *render_mustache(const char *html_content);
#ifdef __cplusplus
}
#endif
#endif