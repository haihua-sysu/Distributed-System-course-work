//
//  socket.cpp
//  clockSync
//
//  Created by haihua on 1/23/19.
//  Copyright © 2019 haihua. All rights reserved.
//

#include "socket.hpp"
#define DEBUG 0

Socket::Socket() : socketfd(-1) {}

Socket::Socket(int socketfd) {
    this->socketfd = socketfd;
}

Socket::~Socket() {
    Close();
}

int Socket::Connect(const char* ip, unsigned short port) {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        puts("client create socket error");
        return -1;
    }
#ifdef DEBUG
    printf("client: ip %d\n", server_addr.sin_addr.s_addr);
    printf("client: socketfd %d\n", socketfd);
#endif
    
    if (connect(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
#ifdef DEBUG
        puts("connect erro");
#endif
        return -1;
    }
    return 0;
}

int Socket::Close() {
    close(socketfd);
	printf("socket closed fd = %d\n", socketfd);
    return 0;
}

////////////////////////////////////////////////ServerSocket

ServerSocket::ServerSocket() : listenfd(-1) {
}

ServerSocket::~ServerSocket() {
    close(listenfd);
}

int ServerSocket::Listen(const char* ip, unsigned short port) {
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
#ifdef DEBUG
        puts("create socket error");
#endif
        return -1;
    }
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    
    if ((bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)){
#ifdef DEBUG
        puts("bind error");
#endif
    }
    
    if (listen(listenfd, 200) == -1){
#ifdef DEBUG
        puts("listen error");
#endif
    }
    return 0;
}

Socket* ServerSocket::Accept() {
    int connfd;
    struct  sockaddr client_info;
    socklen_t len;
    if ((connfd = accept(listenfd, &client_info, &len)) == -1){
#ifdef DEBUG
        printf("accept error listenfd = %d", listenfd);
#endif
        return NULL;
    }
    return new Socket(connfd);
}
