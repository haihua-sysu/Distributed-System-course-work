//
//  main.cpp
//  clockSync
//
//  Created by haihua on 1/23/19.
//  Copyright © 2019 haihua. All rights reserved.
//

#include <iostream>
#include "socket.hpp"
#include <sstream>
#include <string>
#include <thread>
#include <signal.h>

using namespace std;

void delay(double delayTime) {
    time_t last_time, current_time;
    time(&last_time);
    
    time(&current_time);
    double time_gap = 0;
    while ((time_gap = difftime(current_time, last_time)) < delayTime) {
        sleep(1);
        time(&current_time);
    }
}

long getRandomDelivery() {
    return rand() % 500 + 100;
}

void processFun(ServerSocket &server, Socket &serClient, Socket* client) {
	char buffer[1024] = {0};
	while (true) {
		read( client->socketfd , buffer, 1024);
		printf("Processor: %s\n",buffer );

		long delayTime1 = getRandomDelivery();

		string msg = "Proccessor: Time Request";
		send(serClient.socketfd, msg.c_str(), msg.length(), 0);

		char buffer1[1024] = {0};
		read(serClient.socketfd, buffer1, 1024);
		printf("Processor: receive msg from server: currentTime is %s\n",buffer1 );

		long delayTime2 = getRandomDelivery();

		string timeMsg = string(buffer1) + "," + to_string(delayTime1 + delayTime2);
		send(client->socketfd, timeMsg.c_str(), timeMsg.length(), 0);
	}
}

int main(int argc, const char * argv[]) {
    string ip = "127.0.0.1";
    if (argc != 3) {
        printf("Process Usage: <port><Serverport>\n");
        _exit(1);
    }
    
    unsigned int port, serverPort;
    sscanf(argv[1], "%d", &port);
    sscanf(argv[2], "%d", &serverPort);
    
    ServerSocket server;
    if (server.Listen(ip.c_str(), port) < 0) {
        puts("server start error");
        return 0;
    }
    puts("sv1 Server Start\n");
    
    Socket serClient;
    if (serClient.Connect(ip.c_str(), serverPort) < 0) {
        puts("sv1 client connect error");
        return 0;
    }
    puts("sv1 Client Connected\n");
    
    
    while (true) {
        Socket* client = server.Accept();
        if (client == nullptr) {
			puts("sv1 accept error");
			sleep(1);
			continue;
		}
        else {
            thread process(processFun, ref(server), ref(serClient), client);
            process.join();
        }
    }
    
    return 0;
}
