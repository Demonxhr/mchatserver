#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;
// 基于muduo网络库开发服务器程序
// 1.组合TcpServer对象
// 2.创建EventLoop事件循环对象指针
// 3.明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
// 4.在当前服务器类的构造函数中，注册处理连接回调函数和处理读写事件的回调函数
// 5.设置合适的服务端线程数量，muduo库会自己分配io和worker线程数
class Chatserver
{
public:
   Chatserver(EventLoop *loop,               // 事件循环
              const InetAddress &listenAddr, // ip+port
              const string &nameArg)         // 服务器名字
       : _server(loop, listenAddr, nameArg), _loop(loop)
   {
      // 给服务器注册用户连接的创建和断开回调
      _server.setConnectionCallback(std::bind(&Chatserver::onConnection, this, _1));
      // 给服务器注册用户读写事件回调
      _server.setMessageCallback(std::bind(&Chatserver::onMessage, this, _1, _2, _3));
      // 设置服务器端的线程数量 1个io线程 3个worker线程
      _server.setThreadNum(4);
   }
   // 开启事件循环
   void start()
   {
      _server.start();
   }

private:
   void onConnection(const TcpConnectionPtr &conn)
   {

      if (conn->connected())
      {
         cout << conn->peerAddress().toIpPort() << " -> "
              << conn->localAddress().toIpPort()
              << " state:online"
              << endl;
      }
      else
      {
         cout << conn->peerAddress().toIpPort() << " -> "
              << conn->localAddress().toIpPort()
              << " state:offline"
              << endl;
         conn->shutdown();
         //_loop->quit();
      }
   }
   void onMessage(const TcpConnectionPtr &conn, // 连接
                  Buffer *buffer,               // 缓冲区
                  Timestamp time)               // 接收到数据的时间信息
   {
      string buf = buffer->retrieveAllAsString();
      cout << "recv data: " << buf
           << " time: " << time.toString()
           << endl;
      conn->send(buf);
   }
   TcpServer _server;
   EventLoop *_loop;
};

int main()
{
   EventLoop loop;
   InetAddress addr("127.0.0.1", 6000);
   Chatserver server(&loop, addr, "ChatServer");
   server.start();
   loop.loop(); // epoll_wait 以阻塞的方式等待新用户的连接 ，已连接的用户读写事件 等操作
   return 0;
}
