#pragma once

#define _CRT_SECURE_NO_WARNINGS 1
#include <WinSock2.h>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <memory>
#include "HTTPInit.h"
#include "HTTPQueueTasks.h"

#define MAX_CLIENT_BUFFER_SIZE 1024

class HTTPThreadPool
{
private:
  std::vector<std::thread> threads;
  std::mutex mutex_cond;
  std::condition_variable cond;
  HTTPQueueTasks queue;
public:
  explicit HTTPThreadPool(unsigned int _count);
  void insert_task(SOCKET _task);
  void execute_task();
};