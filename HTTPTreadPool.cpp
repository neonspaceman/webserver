#include "HTTPThreadPool.h"

HTTPThreadPool::HTTPThreadPool(unsigned int _count)
  : queue(&cond)
  , threads(_count)
{
  for (size_t i = 0; i < threads.size(); ++i){
    threads[i] = std::thread(&HTTPThreadPool::execute_task, this);
    threads[i].detach();
  }
}

void HTTPThreadPool::insert_task(SOCKET _task)
{
  queue.insert(_task);
}

void HTTPThreadPool::execute_task()
{
  SOCKET client;
  char buffer[MAX_CLIENT_BUFFER_SIZE];
  std::stringstream response;
  std::stringstream response_body;

  printf("[%i] Execute treads\n", std::this_thread::get_id());

  INFINITY_LOOP {
    printf("[%i] Watting tasks...\n", std::this_thread::get_id());

    {
      std::unique_lock<std::mutex> lock(mutex_cond);
      cond.wait(lock);
    }

    if (!queue.get_next(client))
      continue;
    
    printf("[%i] Get task %I64i\n", std::this_thread::get_id(), client);
    
    printf("[%i] Getting request...\n", std::this_thread::get_id());
    // std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    //INFINITY_LOOP 
    {
      int result = 0;
      {
        std::unique_lock<std::mutex> lock(mutex_cond);
        result = recv(client, buffer, MAX_CLIENT_BUFFER_SIZE, 0);
      }
      if (result == SOCKET_ERROR) {
        printf("[%i] Failed (%i)\n", std::this_thread::get_id(), WSAGetLastError());
        break;
      } else if (result == 0) {
        printf("[%i] Disconect\n", std::this_thread::get_id());
        break;
      } else {
        buffer[result] = '\0';
        printf("[%i] Size of data %i\n%s", std::this_thread::get_id(), result, buffer);
        response_body
          << "<title>Test C++ HTTP Server</title>\n"
          << "<h1>Test page</h1>\n"
          << "<p>This is body of the test page...</p>\n"
          << "<h2>Request headers</h2>\n"
          << "<pre>" << buffer << "</pre>\n"
          << "<em><small>Test C++ Http Server</small></em>\n";
        response
          << "HTTP/1.1 200 OK\r\n"
          << "Version: HTTP/1.1\r\n"
          << "Connection: close\r\n"
          << "Content-Type: text/html; charset=utf-8\r\n"
          << "Content-Length: " << response_body.str().length()
          << "\r\n\r\n"
          << response_body.str();

        printf("[%i] Sending response... ", std::this_thread::get_id());
        {
          std::unique_lock<std::mutex> lock(mutex_cond);
          result = send(client, response.str().c_str(), response.str().length(), 0);
        }
        if (result == SOCKET_ERROR) {
          printf("failed (%i)\n", WSAGetLastError());
        }
        printf("ok\n");
      }
    }

    closesocket(client);
  }
}