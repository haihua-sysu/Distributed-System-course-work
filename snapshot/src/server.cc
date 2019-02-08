//
//  server_2.cpp
//  snapshot
//
//  Created by haihua on 1/24/19.
//  Copyright © 2019 haihua. All rights reserved.
//

#include "lib/time.h"
#include "lib/socket.h"

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <signal.h>
#include <stdio.h>


#include "lib/network.pb.h"

#define debug 1

using namespace std;
vector<int> vec(3, 0);

NetworkRequest parseString(string& s) {
    NetworkRequest newRequest;
    newRequest.ParseFromString(s);
    return newRequest;
}

void processFun(ServerSocket& server, Socket* client) {
    //Read Identity
    char buffer[1024] = {0};
    int sz = read( client->socketfd , buffer, 1024);
    printf("server read %d bytes\n", sz);
    string str = string(buffer + 4, sz - 4);

    NetworkRequest tmp = parseString(str);
    if (tmp.type() == NetworkRequest::IDENTITY) vec[tmp.client_from() - 1] = client->socketfd;
    printf("Type %d sent by %d to %d\n", tmp.type(), tmp.client_from(), tmp.client_to());
    
    while (true) {
        memset(buffer, 0 , sizeof(buffer));
        
        /*
        int readByte = read( client->socketfd , buffer, 1024);
        if (readByte == 0) {
            printf("Client closed\n");
            client->Close();
        }*/
        
        int readByte = 0;
        while (readByte == 0) {
            readByte = read( client->socketfd , buffer, 1024);
        }

        printf("Type %d sent by %d to %d, byte is %d\n", tmp.type(), tmp.client_from(), tmp.client_to(), readByte);

        str = string(buffer + 4, readByte - 4);
        tmp = parseString(str);
        
        int writeByte = 0;
        if (tmp.type() == NetworkRequest::TRANSFER || tmp.type() == NetworkRequest::SNAPSHOT) {
            writeByte = write(vec[tmp.client_to() - 1], buffer, readByte);
            //fflush(vec[tmp.client_to() - 1]);
            printf("Send to %d %d byte\n", tmp.client_to(), writeByte);
        } else {
            for (int i = 0; i < vec.size(); i++) {
                if (i + 1 != tmp.client_from()) {
                    writeByte = write(vec[i], buffer, readByte);
                    //fflush(vec[i]);
                    printf("Send to %d %d byte\n", i + 1, writeByte);
                }
            }
        }
        //usleep(300000);
    }
}

int main(int argc, const char * argv[]) {
    string ip = "127.0.0.1";
    if (argc != 2) {
        printf("Usage: <port>\n");
        _exit(1);
    }
    
    unsigned int port;
    sscanf(argv[1], "%d", &port);
    
    ServerSocket server;
    if (server.Listen(ip.c_str(), port) < 0) {
        printf("server start error");
        return 0;
    }
    printf("Server Start\n");
    
    while (true) {
        Socket* client = server.Accept();
        if (client == nullptr) {
            sleep(1);
            continue;
        }
        else {
            
            thread process(processFun, ref(server), client);
            process.detach();
        }
    }
    
    return 0;
}
