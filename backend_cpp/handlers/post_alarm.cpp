#include "post_alarm.h"
#include <iostream>
#include <string>
#include "tag_repository.h"
#include "../lib/mongoose.h"

int resolvehelper(const char* hostname, int family, const char* service, sockaddr_storage* pAddr)
{
    int result;
    addrinfo* result_list = NULL;
    addrinfo hints = {};
    hints.ai_family = family;
    hints.ai_socktype = SOCK_DGRAM; // without this flag, getaddrinfo will return 3x the number of addresses (one for each socket type).
    result = getaddrinfo(hostname, service, &hints, &result_list);
    if (result == 0)
    {
        //ASSERT(result_list->ai_addrlen <= sizeof(sockaddr_in));
        memcpy(pAddr, result_list->ai_addr, result_list->ai_addrlen);
        freeaddrinfo(result_list);
    }

    return result;
}

void postAlarm(std::string macAddressOfTag) {
  std::cout << "Alarm to '" << macAddressOfTag << "' was invoked" << std::endl;

  struct TagInfo tag = getTag(macAddressOfTag);
  sendAlarmPackageTo(tag.IP);
}

void response_handler(struct mg_connection *nc, int ev, void *ev_data) {}

static char * ALARM_MSG = "ALARM";

void sendAlarmPackageTo(std::string ip) {
  int socket_info = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_info == -1) {
    printf("Could not create socket");
  }

  struct sockaddr_in server;
  server.sin_addr.s_addr = inet_addr("192.168.43.132");
  server.sin_family = AF_INET;
  server.sin_port = htons(8003);

  if (connect(socket_info, (struct sockaddr *) &server, sizeof(server)) < 0) {
    perror("Connection error");
  }
  puts("connected");

  char* message = "ALARM";

  if (send(socket_info, message, strlen(message), 0) < 0) {
    perror("Send failed");
    return;
  }
  puts("Message sent");

  close(socket_info);
  /*
  struct mg_mgr mgr;
  mg_mgr_init(&mgr, NULL);
  char addr[256];
  snprintf(addr, sizeof(addr), "udp://localhost:8003");
  struct mg_connection *nc = mg_connect(&mgr, addr, response_handler);

  if(nc != NULL)
  std::cout << "Connection successful" << std::endl;

  mg_send(nc, ALARM_MSG, strlen(ALARM_MSG));

  for (int i = 0; i < 100; i++) {
    mg_mgr_poll(&mgr, 10);
    std::cout << i << std::endl;
  }
  mg_mgr_free(&mgr);
  */
}