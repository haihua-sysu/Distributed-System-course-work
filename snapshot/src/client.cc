//
//  client.cpp
//  snapshot
//
//  Created by haihua on 1/24/19.
//  Copyright © 2019 haihua. All rights reserved.
//

#include "client.h"
#include "lib/socket.h"
#include "lib/time.h"
#include "lib/network.pb.h"

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <time.h>       /* time_t, struct tm, time, localtime */
#include <sys/time.h>
#include "lib/runnable.h"
#include <queue>
#include <map>
#include <vector>

using namespace std;

#define debug 1

int balance = 1000;
int client_id;
unordered_map<int, queue<NetworkRequest>> clientMsg;
bool waitingSnapshot = false;
vector<bool> rcvFlag(3, false);
vector<NetworkRequest> snapshotRecord(3);

// Task override the Runnable interface which allow the timer execute run() method.
// Task has its parameter and implement the execution logic by overriding the pure virtual method run()

class Task : public Runnable {
    public:
    Task(int i): idx(i) {}
    
    void run() override {

        NetworkRequest cur = clientMsg[idx].front();
#if debug
        printf("Callback: type %d\n", cur.type());
#endif
        if (cur.type() == NetworkRequest::TRANSFER) {
#if debug
            printf("transfer\n");
#endif
            balance += cur.money();
            clientMsg[idx].pop();
            printf("Client%d received %d dollars from Client%d. The current balance for Client%d is %d\n", client_id, cur.money(), cur.client_from(), client_id, balance);
#if debug
            printf("transfer end money is %d\n", balance);
#endif
        }
    }
    private:
    int idx;
};

vector<string> splitBySpace(string &s) {
    vector<string> res;
    int i = 0;
    while (i < s.length()) {
        string tmp = "";
        while (i < s.length() && s[i] != ' ') {
            tmp += s[i];
            i++;
        }
        res.push_back(tmp);
        //space
        i++;
    }
    return res;
}

NetworkRequest parseString(string& s) {
    NetworkRequest newRequest;
    newRequest.ParseFromString(s);
    return newRequest;
}

NetworkRequest setNetworkRequest() {
    NetworkRequest request;
    request.set_client_from(client_id);
    request.set_client_to(99);
    request.set_type(NetworkRequest::SNAPSHOT);
    
    request.set_money(balance);
    if (clientMsg.empty()) return request;

    for (auto it = clientMsg.begin(); it != clientMsg.end(); it++) {
        int sum = 0;
        queue<NetworkRequest> tmp = it->second;
        while (!tmp.empty()) {
            NetworkRequest cur = tmp.front();
            if (cur.type() == NetworkRequest::TRANSFER) sum += cur.money();
            tmp.pop();
        }
        if (it == clientMsg.begin()) {
            request.set_channel1_from(it->first + 1);
            request.set_channel1_to(client_id);
            request.set_channel1_money(sum);
        } else {
            request.set_channel2_from(it->first + 1);
            request.set_channel2_to(client_id);
            request.set_channel2_money(sum);
        }
    }
    return request;
}

void formatPrint() {
    printf("formatprint\n");
    for (int i = 0; i < rcvFlag.size(); i++) {
        if (i != client_id - 1 && !rcvFlag[i]) {
            printf("%d cause the error\n", i + 1);
            return;
        }
    }
    NetworkRequest request = setNetworkRequest();
    snapshotRecord[client_id - 1] = request;
    printf("A snapshot has been terminated. The bank status is as following:\n");
    int cnt = 0;
    for (int i = 0; i < snapshotRecord.size(); i++) {
        printf("(%d) The current balance for Client%d is %d.\n", cnt++, i + 1, snapshotRecord[i].money());
        if (snapshotRecord[i].channel1_money() != 0) {
            printf("(%d) (money on the fly) %d dollars is being sent from Client%d to Client%d.\n", cnt++,  snapshotRecord[i].channel1_money(), snapshotRecord[i].channel1_from(), snapshotRecord[i].channel1_to());
        }
        if (snapshotRecord[i].channel2_money() != 0) {
            printf("(%d) (money on the fly) %d dollars is being sent from Client%d to Client%d.\n", cnt++,  snapshotRecord[i].channel2_money(), snapshotRecord[i].channel2_from(), snapshotRecord[i].channel2_to());
        }
        rcvFlag[i] = false;
    }
    rcvFlag[client_id - 1] = true;
}


void receivePro(Socket& client) {
    char buffer[1024] = {0};
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        
        /*
        int readByte = read( client.socketfd , buffer, 1024);
        if (readByte == 0) {
            printf("Client %d connect closed\n", client_id);
            client.Close();
        }*/
        int readByte = 0;
        while (readByte == 0) {
            readByte = read( client.socketfd , buffer, 1024);
        }
        
        string str = string(buffer, readByte);
        NetworkRequest tmp = parseString(str);
        clientMsg[tmp.client_from() - 1].push(tmp);
        
#if debug
        printf("Receive msg from %d, type is %d, byte is %d.\n", tmp.client_from(), tmp.type(), readByte);
#endif
#if 0//debug
        printf("clientmsg %d size is %d.\n", tmp.client_from() - 1, clientMsg[tmp.client_from() - 1].size());
#endif
    }
}


void readMsgFromQueue(Socket& client) {
    while (true) {
        for (auto it = clientMsg.begin(); it != clientMsg.end(); it++) {
            if (!(it->second).empty()) {
                NetworkRequest cur = (it->second).front();

                if (cur.type() == NetworkRequest::MARKER) {
#if debug
                    printf("Marker\n");
#endif
                    NetworkRequest request = setNetworkRequest();
                    
                    request.set_client_to(cur.client_from());
#if 0//debug
                    printf("type is %d, from is %d, to is %d.\n", request.type(), request.client_from(), request.client_to());
#endif
                    // serialize to string
                    string *coding = new string();
                    request.SerializeToString(coding);
                    
                    int writeByte = 0;
                    writeByte = write(client.socketfd, (*coding).c_str(), (*coding).length());
                    
#if debug
                    printf("type is %d, from is %d, to is %d, byte is %d.\n", request.type(), request.client_from(), request.client_to(), writeByte);
#endif
                    
                    (it->second).pop();
                } else if (cur.type() == NetworkRequest::SNAPSHOT) {
#if 1//debug
                    printf("Snapshot from %d\n", cur.client_from());
#endif
                    snapshotRecord[cur.client_from() - 1] = cur;
                    (it->second).pop();
                    rcvFlag[cur.client_from() - 1] = true;
                    formatPrint();
                } else {
                    if (cur.flag() == 0) continue;
                    Runnable* task = new Task(it->first);
                    // task->run() will be call from another thread after 2500 ms.
                    // registerCallback is an unblocked method.
#if debug
                    printf("call callback\n");
#endif
                    registerCallback(task, 15000);
                    (it->second).front().set_flag(0);
                    
                }
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    string ip = "127.0.0.1";
    if (argc != 3) {
        printf("Usage: <client_id><serverPort>\n");
        _exit(1);
    }
    
    unsigned int port;
    sscanf(argv[1], "%d", &client_id);
    sscanf(argv[2], "%d", &port);
    
    rcvFlag[client_id - 1] = true;
    
    Socket client;
    if (client.Connect(ip.c_str(), port) < 0) {
        puts("client connect error");
        return 0;
    }
    printf("Client %d Connected\n", client_id);
    
    //Sent identity
    NetworkRequest request;
    request.set_client_from(client_id);
    request.set_client_to(99);
    request.set_type(NetworkRequest::IDENTITY);
    
    // serialize to string
    string *coding = new string();
    request.SerializeToString(coding);
    
    write(client.socketfd, (*coding).c_str(), (*coding).length());
    
    //Read Msg and msg queue
    thread t1(receivePro, ref(client));
    thread t2(readMsgFromQueue, ref(client));
    t1.detach();
    t2.detach();
    
    while (true) {
        cout << "Please input command:" << endl;
        cout << "Command example:" << endl;
        cout << "Transfer: transfer source dest value." << endl;
        cout << "Snapshot: snapshot source." << endl;
        string command;
        while (getline(cin, command)) {
            NetworkRequest request;
            vector<string> vec = splitBySpace(command);
            if (vec.empty()) continue;
            //Type
            if (vec[0] == "transfer") request.set_type(NetworkRequest::TRANSFER);
            else if (vec[0] == "snapshot") {
                request.set_type(NetworkRequest::MARKER);
                waitingSnapshot = true;
            } else {
                printf("Invalid command.\n");
                break;
            }
            //Source
            if (stoi(vec[1]) != client_id) {
                printf("Source need to be the current client: %d.\n", client_id);
                break;
            } else {
                request.set_client_from(client_id);
            }
            
            request.set_client_to(99);
            
            //Optional: money
            if (vec[0] == "transfer") {
                //Dest
                request.set_client_to(stoi(vec[2]));
                request.set_money(stoi(vec[3]));
                request.set_flag(1);
                balance -= stoi(vec[3]);
                printf("Client%d sent %d dollars to Client%d. The current balance for Client%d is %d.\n", client_id, request.money(), request.client_to(), client_id, balance);
            } else {
                printf("A snapshot has been initiated by Client%d.\n", client_id);
            }
            
            // serialize to string
            string *coding = new string();
            request.SerializeToString(coding);
            
            write(client.socketfd, (*coding).c_str(), (*coding).length());
        }
    }
    
    return 0;
}
