#pragma once
#include "precomp.h"


class Logging
{
  public:
    static void LogInfo(string Info)
    {
        time_t currentTime;
        struct tm* localTime;
        time(&currentTime);
        localTime = localtime(&currentTime);

        printf("[%i:%i:%i] Info | %s\n", localTime->tm_hour, localTime->tm_min, localTime->tm_sec, Info.c_str());
    }
};