#include "HTTPQueueTasks.h"

HTTPQueueTasks::HTTPQueueTasks(std::condition_variable *const _cond)
  : cond(_cond)
{}

void HTTPQueueTasks::insert(SOCKET _task)
{
  std::unique_lock<std::recursive_mutex> lock(mutex);
  queue.push(_task);
  printf("[Server] Insert new task %i (now %i)\n", _task, queue.size());
  cond->notify_one();
}

bool HTTPQueueTasks::get_next(SOCKET &_task)
{
  std::unique_lock<std::recursive_mutex> lock(mutex);
  if (queue.size()) {
    _task = queue.front();
    queue.pop();
    return true;
  }
  return false;
}