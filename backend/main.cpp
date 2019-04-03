#include "./lib/mongoose.h"
#include <vector>
#include "TagInfo.h"
#include "./lib/frozen.h"
#include "./handlers/post_alarm.h"
#include <iostream>
#include <sys/socket.h>
#include "./handlers/tag_repository.h"

static const char *s_http_port = "8000";
static const char *s_udp_port = "udp://8001";

static const struct mg_str s_get_method = MG_MK_STR("GET");
static const struct mg_str s_post_method = MG_MK_STR("POST");

static int is_equal(const struct mg_str *s1, const struct mg_str *s2) {
  return s1 -> len == s2 -> len && memcmp(s1 -> p, s2 -> p, s2 -> len) == 0;
}

void print_tag_info_json(struct json_out *out, va_list *ap) {
  std::vector<struct TagInfo> *vec = va_arg(*ap, std::vector<struct TagInfo> *);

  json_printf(out, "[");

  for (int i = 0; i < vec -> size(); i++) {
    struct TagInfo p = (*vec)[i];

    json_printf(
      out,
      "{ name: \"%s\", macAddress: \"%s\", lastSeen: %d, isConnected: %B }",
      p.name.c_str(),
      p.macAddress.c_str(),
      p.lastSeen,
      p.isConnected
    );

    if (i < vec -> size() - 1) {
      json_printf(out, ", ");
    }
  }

  json_printf(out, "]");
}

static void ev_handler(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    struct http_message *hm = (struct http_message *) p;

    std::cout << hm -> uri.p << std::endl;

    if (is_equal(&hm -> method, &s_post_method)) {
      char id[1024];
      int result = mg_get_http_var(&hm -> query_string, "id", id, sizeof(id));

      if (result > 0) {
        //send alarm to tag 'id'
        postAlarm(id);
        mg_send_head(c, 200, 0, "Access-Control-Allow-Origin: *");
        return;
      } else {
        mg_send_head(c, 400, 1024, "Access-Control-Allow-Origin: *");
        mg_printf(c, " ");
        return;
      }
    } else if (is_equal(&hm -> method, &s_get_method)) {

      std::vector<TagInfo> allTags;
      getAllTags(&allTags);

      char response_body[2048];
      struct json_out out = JSON_OUT_BUF(response_body, sizeof(response_body));
      json_printf(&out, "%M", print_tag_info_json, &allTags);

      mg_send_head(c, 200, strlen(response_body), "Access-Control-Allow-Origin: *");
      
      mg_printf(c, response_body);
      return;
    }
  }
}

void tokenize(std::string const &str, const char delim,
			std::vector<std::string> &out)
{
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
}


static void udp_ev_handler(struct mg_connection *c, int ev, void *p) {
  struct mbuf *io = &c->recv_mbuf;
  
  if (ev == MG_EV_RECV) {


    char *buf = (char*) calloc(io->len + 1, 1);
    memcpy(buf, io->buf, io->len);
    mbuf_remove(io, io->len);

    char IP[100];
    mg_sock_addr_to_str(&c -> sa, IP, sizeof(IP), MG_SOCK_STRINGIFY_REMOTE | MG_SOCK_STRINGIFY_IP);

    std::vector<std::string> out;
	  tokenize(buf, ';', out);

    bool isTagFinderPacket = out[0] == "TAGFINDER";
    if (isTagFinderPacket) {
      std::string macAddress = out[1];
      std::string tagName = out[2];

      struct TagInfo newTag = { tagName, IP, true, macAddress, 0 };

      addTag(newTag);
    }

    c->flags |= MG_F_SEND_AND_CLOSE;
  }
  
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *c;
  struct mg_connection *c2;

  mg_mgr_init(&mgr, NULL);
  c = mg_bind(&mgr, s_http_port, ev_handler);
  mg_set_protocol_http_websocket(c);

  c2 = mg_bind(&mgr, s_udp_port, udp_ev_handler);

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
