#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#ifdef __cplusplus
extern "C" {
#endif

void mg_http_serve_dir_ext(struct mg_connection *c, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *opts);

#ifdef __cplusplus
}
#endif
#endif