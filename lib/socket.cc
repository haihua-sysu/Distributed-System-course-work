//
//  socket.cpp
//  clockSync
//
//  Created by haihua on 1/23/19.
//  Copyright © 2019 haihua. All rights reserved.
//

#include "socket.h"

int read_len(char* buffer, int &cur) {
    int ret = 0;
    for (int i = 0; i < 4; i++) {
        ret = ret << 8;
        ret |= buffer[cur++];
    }
    return ret;
}

Socket::Socket() : socketfd(-1) {}

Socket::Socket(int socketfd) {
    this->socketfd = socketfd;
}

Socket::~Socket() {}

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

string Socket::recvMessage() {
    if (msg.empty()) {
        char buffer[1024];
        int bytes = recv(socketfd, buffer, 1024, 0);
        int cur = 0;
        char* pBuf = buffer;
        while (cur < bytes) {
            int msg_len = read_len(pBuf, cur);
            string str(pBuf + cur, msg_len);
            pBuf += cur + msg_len;
            cur += msg_len;
        }
    }

    if (!msg.empty()) {
        string ret = msg.front();
        msg.pop();
        return ret;
    }

    // return empty string if any error occurred.
    return "";
}

void Socket::sendMessage(const string &str) {
    int len = str.length();
    char buffer[1024];
    for (int i = 3; i >= 0; i--) {
        buffer[i] = len & 255;
        len >>= 8;
    }
    strncpy(buffer + 4, str.c_str(), str.length());
    send(socketfd, buffer, 4 + str.length(), 0);
}

int Socket::Close() {
    close(socketfd);
    printf("socket closed fd = %d\n", socketfd);
    return 0;
}

////////////////////////////////////////////////ServerSocket

ServerSocket::ServerSocket() : listenfd(-1) {}

ServerSocket::~ServerSocket() {}

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
