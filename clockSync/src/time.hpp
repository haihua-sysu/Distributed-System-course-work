//
//  time.hpp
//  clockSync
//
//  Created by haihua on 1/24/19.
//  Copyright © 2019 haihua. All rights reserved.
//

#ifndef time_hpp
#define time_hpp

#include <stdio.h>
#include <time.h>       /* time_t, struct tm, time, localtime */
#include <sys/time.h>
#include <string>

using namespace std;

long getCurrentTime();

bool setCurrentTime(struct timeval tv, long changeTime);

long diffTime(struct timeval tv1, struct timeval tv2);

long diffTimeInMillisecond(long t1, long t2);

string formatCurrentTime();

void convertStringToTime(char buffer[], struct timeval &utcTime, long &delivery);

string convertTimeToString(struct timeval utcTime, long delivery);

#endif /* time_hpp */
