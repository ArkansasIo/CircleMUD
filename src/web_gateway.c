#include "web_gateway.h"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

static int set_nonblocking(int fd)
{
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0)
    return -1;
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

static void close_client(struct aw_web_client *client)
{
  if (client->fd >= 0)
    close(client->fd);
  client->fd = -1;
  client->connected = 0;
  client->websocket = 0;
  client->player_id = 0;
}

static int add_client(struct aw_web_gateway *gateway, int fd)
{
  size_t i;
  for (i = 0; i < AW_WEB_MAX_CLIENTS; ++i) {
    if (!gateway->clients[i].connected) {
      gateway->clients[i].fd = fd;
      gateway->clients[i].connected = 1;
      gateway->clients[i].websocket = 0;
      gateway->clients[i].player_id = 0;
      return (int)i;
    }
  }
  return -1;
}

static int send_http_upgrade_placeholder(int fd)
{
  static const char response[] =
    "HTTP/1.1 426 Upgrade Required\r\n"
    "Connection: close\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 28\r\n\r\n"
    "WebSocket upgrade required.\n";
  return send(fd, response, sizeof(response) - 1, 0) == (ssize_t)(sizeof(response) - 1) ? 0 : -1;
}

int aw_web_gateway_init(struct aw_web_gateway *gateway, uint16_t port)
{
  struct sockaddr_in address;
  int opt = 1;
  size_t i;

  if (!gateway || port == 0)
    return -1;

  memset(gateway, 0, sizeof(*gateway));
  gateway->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (gateway->listen_fd < 0)
    return -1;

  (void)setsockopt(gateway->listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(port);

  if (bind(gateway->listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0 ||
      listen(gateway->listen_fd, 32) < 0 || set_nonblocking(gateway->listen_fd) < 0) {
    close(gateway->listen_fd);
    gateway->listen_fd = -1;
    return -1;
  }

  gateway->port = port;
  gateway->running = 1;
  for (i = 0; i < AW_WEB_MAX_CLIENTS; ++i)
    gateway->clients[i].fd = -1;
  return 0;
}

int aw_web_gateway_poll(struct aw_web_gateway *gateway, int timeout_ms)
{
  fd_set readfds;
  struct timeval timeout;
  int result;
  int max_fd;
  size_t i;

  if (!gateway || !gateway->running || gateway->listen_fd < 0)
    return -1;

  FD_ZERO(&readfds);
  FD_SET(gateway->listen_fd, &readfds);
  max_fd = gateway->listen_fd;
  for (i = 0; i < AW_WEB_MAX_CLIENTS; ++i) {
    if (gateway->clients[i].connected) {
      FD_SET(gateway->clients[i].fd, &readfds);
      if (gateway->clients[i].fd > max_fd)
        max_fd = gateway->clients[i].fd;
    }
  }

  timeout.tv_sec = timeout_ms / 1000;
  timeout.tv_usec = (timeout_ms % 1000) * 1000;
  result = select(max_fd + 1, &readfds, NULL, NULL, &timeout);
  if (result < 0) {
    if (errno == EINTR)
      return 0;
    return -1;
  }
  if (result == 0)
    return 0;

  if (FD_ISSET(gateway->listen_fd, &readfds)) {
    int client_fd = accept(gateway->listen_fd, NULL, NULL);
    if (client_fd >= 0) {
      if (set_nonblocking(client_fd) < 0 || add_client(gateway, client_fd) < 0) {
        close(client_fd);
      }
    }
  }

  for (i = 0; i < AW_WEB_MAX_CLIENTS; ++i) {
    char buffer[AW_WEB_MAX_FRAME];
    ssize_t bytes;
    if (!gateway->clients[i].connected || !FD_ISSET(gateway->clients[i].fd, &readfds))
      continue;

    bytes = recv(gateway->clients[i].fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) {
      close_client(&gateway->clients[i]);
      continue;
    }
    buffer[bytes] = '\0';

    /* The production WebSocket handshake/frame implementation is deliberately
     * kept behind this boundary. This transport accepts connections but never
     * treats raw HTTP as game commands, preventing accidental protocol mixing.
     */
    if (!gateway->clients[i].websocket) {
      (void)send_http_upgrade_placeholder(gateway->clients[i].fd);
      close_client(&gateway->clients[i]);
    }
  }

  return 0;
}

int aw_web_gateway_send_text(struct aw_web_gateway *gateway, int client_index, const char *text)
{
  (void)gateway;
  (void)client_index;
  (void)text;
  return -1;
}

void aw_web_gateway_shutdown(struct aw_web_gateway *gateway)
{
  size_t i;
  if (!gateway)
    return;
  gateway->running = 0;
  for (i = 0; i < AW_WEB_MAX_CLIENTS; ++i)
    close_client(&gateway->clients[i]);
  if (gateway->listen_fd >= 0)
    close(gateway->listen_fd);
  gateway->listen_fd = -1;
}
