//
//  client.cpp
//  clockSync
//
//  Created by haihua on 1/24/19.
//  Copyright © 2019 haihua. All rights reserved.
//

#include "client.h"
#include "lib/socket.h"
#include "lib/time.h"

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <time.h>       /* time_t, struct tm, time, localtime */
#include <sys/time.h>

using namespace std;

int main(int argc, const char * argv[]) {
    string ip = "127.0.0.1";
    if (argc != 3) {
        printf("Usage: <port><client_id>\n");
        _exit(1);
    }
    
    unsigned int port, client_id;
    sscanf(argv[1], "%d", &port);
    sscanf(argv[2], "%d", &client_id);
    
    int drift, tolerate_time;
    printf("Usage: <time_drift in millisecond>: ");
    scanf("%d", &drift);
    printf("\nUsage: <tolerate_time in millisecond>: ");
    scanf("%d", &tolerate_time);
	printf("%d %d\n", drift, tolerate_time);
    
    Socket client;
    if (client.Connect(ip.c_str(), port) < 0) {
        puts("client connect error");
        return 0;
    }
    puts("Client Connected\n");
    
    while (true) {
        string currentTime = formatCurrentTime();
        printf("Client%d: %s", client_id, currentTime.c_str());
        
        string msg = "Client" + to_string(client_id) + ": Time Request";
        
        send(client.socketfd, msg.c_str(), msg.length(), 0);
        long long last_time, current_time;
        last_time = getCurrentTime();
        
        char buffer[1024] = {0};
        read( client.socketfd , buffer, 1024);
        
        struct timeval utcTime;
        long delivery;
        convertStringToTime(buffer, utcTime, delivery);
        
        setCurrentTime(utcTime, 0);
        currentTime = formatCurrentTime();
        
        printf("Client%d: UTC time is %s, Delivery time is %ld", client_id, currentTime.c_str(), delivery);
        
        setCurrentTime(utcTime, delivery);
        
        current_time = getCurrentTime();
        long long time_gap = 0;
        while ((time_gap = diffTimeInMillisecond(last_time, current_time)) < tolerate_time) {
            printf("Client%d: Request of Time will be sent after %d millisecond", client_id, tolerate_time - time_gap);
            setCurrentTime(utcTime, drift);
			printf("Client%d: sleep for %d milliseconds\n", client_id, drift);
            this_thread::sleep_for (chrono::milliseconds(drift));
            printf("Client%d: %d milliseconds passed\n\n", client_id, client_id == 1 ? drift : (-drift));
            current_time = getCurrentTime();
        }
    }
    
    return 0;
}
