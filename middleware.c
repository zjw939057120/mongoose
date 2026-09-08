#include "mongoose.h"
#include "middleware.h"

void mg_http_serve_dir_ext(struct mg_connection *c, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *opts)
{
    char path[MG_PATH_MAX];
    int flags = mg_url_decode(hm->uri.buf, hm->uri.len, 
                            path, sizeof(path), 0);
    printf("flags: %d, path: %s\n", flags, path);
    if(mg_casecmp(path, "/") == 0)
    {
        mg_http_reply(c, 302, "Location: /index.html\r\n", "");
        return;
    }
    const char *html_suffix = ".html";
    size_t suffix_len = strlen(html_suffix);
    size_t path_len = strlen(path);
    if (path_len >= suffix_len && memcmp(path + path_len - suffix_len, html_suffix, suffix_len) == 0) {
      mg_http_serve_file(c, hm, "web_root/page.html", opts);
      return;
    }
    mg_http_serve_dir(c, hm, opts);
}
