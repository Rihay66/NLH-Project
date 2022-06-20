#include "iostream"
#include <thread>
#include <ctime>
#include <chrono>

using namespace std::chrono_literals;

//This class can be called and it will return a value and continue until program exits
class timeManager{
    private:
        long long currentTime;
    public:
        bool runClock;
        void setTime();
        long long callTime();
};