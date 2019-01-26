//
//  time.cpp
//  clockSync
//
//  Created by haihua on 1/24/19.
//  Copyright © 2019 haihua. All rights reserved.
//

#include "time.h"
#include <time.h>       /* time_t, struct tm, time, localtime */
#include <sys/time.h>
#include <string>
#include <vector>
using namespace std;

long getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*1000+tv.tv_usec/1000; //return time in millisecond
}

bool setCurrentTime(struct timeval tv, long changeTime) {
    tv.tv_usec = changeTime * 1000;
    if (settimeofday(&tv, NULL)) return true;
    return false;
}

long diffTime(struct timeval tv1, struct timeval tv2) {
    return (tv2.tv_sec*1000+tv2.tv_usec/1000) - (tv1.tv_sec*1000+tv1.tv_usec/1000);
}

long diffTimeInMillisecond(long t1, long t2) {
    return t2 - t1;
}

string formatCurrentTime() {
    time_t rawtime;
    struct tm *timeinfo;
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    
    struct timeval tv;
    gettimeofday(&tv,NULL);
    
    string res = asctime (timeinfo) + to_string(tv.tv_usec/1000);
    
    return res;
}

void convertStringToTime(char buffer[], struct timeval &utcTime, long &delivery) {
    string res(buffer);
    
    int start = 0;
    size_t found;
    vector<long> vec;
    while ((found = res.find(',', start)) != string::npos) {
        string tmpS = res.substr(start, found - start);
        vec.push_back(stol(tmpS));
        start = found + 1;
    }
    utcTime.tv_sec = vec[0];
    utcTime.tv_usec = vec[1];
    delivery = stol(res.substr(start));
}

string convertTimeToString(struct timeval utcTime, long delivery) {
    string res;
    res = to_string(utcTime.tv_sec) + "," + to_string(utcTime.tv_usec);
    if (delivery != 0) res += "," + to_string(delivery);
    return res;
}
