#pragma once
#include <stdint.h>

class TimerRNG {
    public:
        TimerRNG(int64_t Minimum, int64_t Maximum);
        void UpdateRNGTimer();

        // Getters
        int64_t GetRandomNumber();
    private:
        int64_t RNGTimer;
        int64_t Minimum;
        int64_t Maximum;
};