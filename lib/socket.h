//
//  socket.hpp
//  clockSync
//
//  Created by haihua on 1/23/19.
//  Copyright © 2019 haihua. All rights reserved.
//

#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

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

class ServerSocket {
public:
    ServerSocket();
    
    ~ServerSocket();
    
    /**
     * Start to listen.
     */
    int Listen(const char* ip, unsigned short port);
    
    /**
     * Accept a connect request and return a socket pointer that associated with the request.
     */
    Socket* Accept();

    /** Close the socket. This should be called explicitly when the socket is no longer need. */
    void Close();
    
//private:
    int listenfd;
};

#endif /* socket_hpp */
