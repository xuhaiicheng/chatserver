#include "chatserver.hpp"
#include "chatservice.hpp"
#include <iostream>
#include <signal.h>
using namespace std;

//处理服务器ctrl+c结束后。重置user的状态信息
void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}

int main(int argc, char **argv)
{
    signal(SIGINT, resetHandler);

    //解析通过命令行参数传递的ip和port
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);

    EventLoop loop;
    InetAddress addr(ip, port);
    ChatServer server(&loop, addr, "ChatServer");

    server.start();
    loop.loop();
    //以上时网络模块的代码，直接使用muduo库，通过事件驱动的、I/o epoll多路复用+线程池的网络,基于Reactor。

    return 0;
}