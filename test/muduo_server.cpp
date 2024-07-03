/*
muduo网络库给用户提供了两个主要的类
TcpServer:用于编写服务器程序的
TcpClient:用于编写客户端程序的

epoll+线程池
好处：能够把网络I/O的代码和业务代码区分开
业务代码：用户的连接和断开  用户的可读写事件

*/

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <string>
#include <functional>  
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

/*基于muduo网络库开发服务器程序
1. 组合TcpServer对象
2. 创建EventLoop事件循环对象的指针
3. 明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
4. 在当前服务器类的构造函数中，注册处理连接的回调函数和处理读写事件的回调函数
5. 设置合适的服务端线程数量，muduo库会自己划分I/O线程和worker线程
*/
class ChatServer {

public:
        ChatServer(EventLoop* loop,      //事件循环(Reactor)
            const InetAddress& listenAddr,  //IP+Port
            const string& nameArg)       //服务器的名字
            :_server(loop,listenAddr, nameArg),_loop(loop)
    {
        //给服务器注册用户连接的创建和断开回调  使用回调主要是发生某件事的时间和发生后如何处理不在同一时间
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this, _1));

        //给服务器注册用户读写事件回调  同理，不知道什么时候发生用户读写事件
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));   //_1、_2、_3是参数占位符

        //设置服务器短的线程数量 1个I/O线程  3个worker线程
        _server.setThreadNum(4);

    }

    //开启事件循环
    void start(){
        _server.start();  //TcpServer.h里面有现成的
    }

private:
    //专门处理用户的连接创建和断开    epoll listenfd  accept（这些内容都封装起来了，只暴露一个回调接口给你使用）
    void onConnection(const TcpConnectionPtr& conn) {

        if(conn->connected()){
            cout<< conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort()<< "state:online" << endl;
        }else{
            cout<< conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort()<< "state:offline" << endl;
            conn->shutdown();  //close(fd)
            //_loop->quit();     //退出整个epoll，服务器结束
        }
    }
 
    //专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr &conn,    //连接
                            Buffer *buffer,            //缓冲区
                            Timestamp time)         //接收到数据的时间信息
    {
        string buf = buffer->retrieveAllAsString();
        cout<<"recv data:" << buf << "time" << time.toString() << endl;                   
        conn->send(buf);             
                            
    }

    TcpServer _server;  //#1
    EventLoop *_loop;    //#2 epoll

};

int main(){
    
    EventLoop loop; //epoll
    InetAddress addr("127.0.0.1",6000);
    ChatServer server(&loop, addr, "ChatServer");

    server.start(); //启动服务  将listenfd 通过epoll_ctl 添加到epoll上
    loop.loop();   //epoll_wait以阻塞方式等待新用户连接，已连接用户的读写事件等
    return 0;
}