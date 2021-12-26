// Copyright (c) 2021 B. Atticus Grobe (grobe0ba@gmail.com)
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <strings.h>

#include "mongoose.h"

#define LSTN "http://0.0.0.0:8080"

static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_http_serve_opts opts = {.fs = &mg_fs_packed};
  struct mg_http_message *hm = (struct mg_http_message *)ev_data;
  if (ev == MG_EV_HTTP_MSG) {
    char url[MG_PATH_MAX];
    bcopy(hm->uri.ptr, url, MG_PATH_MAX);
    url[hm->uri.len] = 0;
    LOG(LL_INFO, ("Serving %s", url));
    mg_http_serve_dir(c, ev_data, &opts);
  }
}

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  if (!mg_http_listen(&mgr, LSTN, cb, &mgr)) {
    return 1;
  }
  LOG(LL_INFO, ("Listening on " LSTN));
  for (;;) mg_mgr_poll(&mgr, 1000);
  LOG(LL_INFO, ("Shutdown"));
  mg_mgr_free(&mgr);
  return 0;
}
