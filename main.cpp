#include <iostream>
#include <thread>
#include "HTTPServer.h"

int main(int argc, char *argv[])
{
  try {
    // std::thread::hardware_concurrency()
    HTTPServer server(2);
    server.start(8088);
  } catch (std::exception &err){
    std::cout << err.what() << std::endl;
  }

  return 0;
}