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
#include <functional>

#include "lib/network.pb.h"
#include "lib/utils.h"

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
    string str = client->recvMessage();

    NetworkRequest tmp = parseString(str);
    if (tmp.type() == NetworkRequest::IDENTITY) vec[tmp.client_from() - 1] = client->socketfd;
    cout << "server recv message = " << messageToJsonString(tmp) << endl;
    
    while (true) {
        str = client->recvMessage();
        if (str.empty()) {
            cout << "client closed" << endl;
            break;
        }

        tmp = parseString(str);
        cout << "server recv message = " << messageToJsonString(tmp) << endl;

        if (tmp.type() == NetworkRequest::TRANSFER || tmp.type() == NetworkRequest::SNAPSHOT) {
            Socket socket(vec[tmp.client_to() - 1]);
            socket.sendMessage(str);
            printf("Send to %d %d byte\n", tmp.client_to(), str.length());
        } else {
            for (int i = 0; i < vec.size(); i++) {
                if (i + 1 != tmp.client_from()) {
                    Socket socket(vec[i]);
                    socket.sendMessage(str);
                    printf("Send to %d %d byte\n", i + 1, str.length());
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
