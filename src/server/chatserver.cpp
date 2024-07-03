#include "chatserver.hpp"
#include "json.hpp"
#include "chatservice.hpp"

#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

//聊天服务器的主类
ChatServer::ChatServer(EventLoop *loop, 
            const InetAddress  &listenAddr, 
            const string  &nameArg)
            :_server(loop, listenAddr, nameArg), _loop(loop)
{
        //注册链接回调
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

        //注册消息回调
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));   

        //设置线程数量
        _server.setThreadNum(4); 
            
}

    //启动服务
    void ChatServer::start()
    {
        _server.start();
    }

    
    //上报链接相关信息的回调函数
    void ChatServer::onConnection(const TcpConnectionPtr& conn)
    {
        //客户端断开链接
        if(!conn->connected()){
            ChatService::instance()->clientCloseException(conn);
            conn->shutdown();  //释放fd资源
        }
    }

    //上报读写事件相关信息的回调函数
    void ChatServer::onMessage(const TcpConnectionPtr &conn,
                            Buffer *buffer ,
                            Timestamp time)
    {

        string buf =buffer->retrieveAllAsString();
        //数据的反序列化
        json js = json::parse(buf);
        //达到的目的：完全解耦网络模块的代码和业务模块的代码
        //通过js["msgid"] 来获取 业务handler =》conn js time
        auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>()); //虽然打印出来是数字，但是类型还是用json中的类型来表示的，用get方法将类型转换成int类型
        //回调消息绑定好的事件处理器，来执行相应的业务处理，因此网络模块的代码和业务模块的代码完全拆分开
        msgHandler(conn,js,time);
    }