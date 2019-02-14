#pragma once

#define _CRT_SECURE_NO_WARNINGS 1
#include <cstdio>
#include <WinSock2.h>
#include "HTTPInit.h"
#include "HTTPThreadPool.h"

#pragma comment(lib, "Ws2_32.lib")

class HTTPServer 
{
private:
  WSADATA wsaData;
  SOCKET server = INVALID_SOCKET;
  SOCKADDR_IN addr;
  HTTPThreadPool pool;
public:
  explicit HTTPServer(unsigned _thread_pool_size);
  void start(unsigned short port);
  void stop();
};