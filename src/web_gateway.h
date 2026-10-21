#ifndef CIRCLE_WEB_GATEWAY_H
#define CIRCLE_WEB_GATEWAY_H

#include <stddef.h>
#include <stdint.h>

#define AW_WEB_MAX_CLIENTS 64
#define AW_WEB_MAX_FRAME 8192

struct aw_web_client {
  int fd;
  uint8_t connected;
  uint8_t websocket;
  uint32_t player_id;
};

struct aw_web_gateway {
  int listen_fd;
  uint16_t port;
  uint8_t running;
  struct aw_web_client clients[AW_WEB_MAX_CLIENTS];
};

int aw_web_gateway_init(struct aw_web_gateway *gateway, uint16_t port);
int aw_web_gateway_poll(struct aw_web_gateway *gateway, int timeout_ms);
int aw_web_gateway_send_text(struct aw_web_gateway *gateway, int client_index, const char *text);
void aw_web_gateway_shutdown(struct aw_web_gateway *gateway);

#endif
