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

#include "lib/network.pb.h"

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
    read( client->socketfd , buffer, 1024);
    string str = string(buffer);
    NetworkRequest tmp = parseString(str);
    if (tmp.type() == NetworkRequest::IDENTITY) vec[tmp.client_from() - 1] = client->socketfd;
    printf("Type %d sent by %d to %d\n", tmp.type(), tmp.client_from(), tmp.client_to());
    
	while (true) {
        memset(buffer, 0 , sizeof(buffer));
        
        while (strlen(buffer) == 0) {
            sleep(1);
            recv( client->socketfd , buffer, 1024, 0);
        }
        str = string(buffer);
		tmp = parseString(str);
        
        printf("Type %d sent by %d to %d\n", tmp.type(), tmp.client_from(), tmp.client_to());
        
        if (tmp.type() == NetworkRequest::TRANSFER || tmp.type() == NetworkRequest::SNAPSHOT) {
            cout << write(vec[tmp.client_to() - 1], buffer, strlen(buffer)) << endl;
        } else {
            for (int i = 0; i < vec.size(); i++) {
                if (i + 1 != tmp.client_from()) write(vec[i], buffer, strlen(buffer));
            }
        }
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
