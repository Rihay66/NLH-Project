#include "../inc/TimeManager.h"

void timeManager::setTime(){
    while(runClock){
        auto time = std::chrono::system_clock::now();
        std::time_t endtime = std::chrono::system_clock::to_time_t(time);

        currentTime = endtime;

        //To avoid cpu idle usage
        std::this_thread::sleep_for(1ms);
    }
}

long long timeManager::callTime(){

    return currentTime;
}