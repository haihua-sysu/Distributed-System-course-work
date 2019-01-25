//
//  socket.hpp
//  clockSync
//
//  Created by haihua on 1/23/19.
//  Copyright © 2019 haihua. All rights reserved.
//

#ifndef socket_hpp
#define socket_hpp

#include <string>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

class Socket {
public:
    /**
     * init
     */
    Socket();
    
    /**
     * init
     */
    Socket(int fd);
    
    /**
     * deconstruct
     */
    ~Socket();
    
    /**
     * connect to server
     */
    int Connect(const char* ip, unsigned short port);
    
    /**
     * close the connection
     */
    int Close();
    
//private:
    int socketfd;
};

/**
 * 服务端 socket
 */
class ServerSocket {
public:
    /**
     * 构造函数
     */
    ServerSocket();
    
    /**
     * 析构函数
     */
    ~ServerSocket();
    
    /**
     * 监听端口
     */
    int Listen(const char* ip, unsigned short port);
    
    /**
     * accept 新连接
     */
    Socket* Accept();
    
//private:
    int listenfd;
};

#endif /* socket_hpp */
