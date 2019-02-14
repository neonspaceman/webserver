#include "HTTPServer.h"

HTTPServer::HTTPServer(unsigned _thread_pool_size)
 : pool(_thread_pool_size)
{}

void HTTPServer::start(unsigned short _port)
{
  int result = 0;

  printf("[Server] initializing lib... ");
  result = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (result != 0) {
    char msg[256];
    sprintf(msg, "failed (%i)\n", result);
    throw new std::exception(msg);
  }
  printf("ok\n");
    
  printf("[Server] creating server socket... ");
  server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server == INVALID_SOCKET) {
    char msg[256];
    sprintf(msg, "failed (%i)\n", WSAGetLastError());
    stop();
    throw new std::exception(msg);
  }
  printf("ok\n");

  printf("[Server] binding socket... ");
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(_port);
  addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(server, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
  {
    char msg[256];
    sprintf(msg, "failed (%i)\n", WSAGetLastError());
    stop();
    throw new std::exception(msg);
  }
  printf("ok\n");

  printf("[Server] listening... ");
  if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
    char msg[256];
    sprintf(msg, "failed (%i)\n", WSAGetLastError());
    stop();
    throw new std::exception(msg);
  }
  printf("ok\n");

  INFINITY_LOOP {
    printf("[Server] Accepting cliens...\n");
    SOCKET client = accept(server, nullptr, nullptr);
    printf("[Server] accept (%i)\n", client);
    if (client == INVALID_SOCKET) {
      char msg[256];
      sprintf(msg, "accept failed: %i\n", WSAGetLastError());
      stop();
      throw new std::exception(msg);
    }
    pool.insert_task(client);
  }
}

void HTTPServer::stop()
{
  closesocket(server);
  WSACleanup();
}