#pragma once

#define _CRT_SECURE_NO_WARNINGS 1

#include <queue>
#include <mutex>
#include <condition_variable>
#include <WinSock2.h>

class HTTPQueueTasks
{
private:
  std::condition_variable *const cond;
  std::recursive_mutex mutex;
  std::queue<SOCKET> queue;
public:
  explicit HTTPQueueTasks(std::condition_variable *const _cond);
  void insert(SOCKET _task);
  // return success of operation
  bool get_next(SOCKET &_task);
};