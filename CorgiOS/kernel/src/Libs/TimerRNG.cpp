#include "TimerRNG.hpp"

TimerRNG::TimerRNG(int64_t Minimum, int64_t Maximum) {
    this->Minimum = Minimum;
    this->Maximum = Maximum;
    this->RNGTimer = 0;
}

void TimerRNG::UpdateRNGTimer() {
    RNGTimer++;

    if (this->RNGTimer > this->Maximum) {
        this->RNGTimer = Minimum;
    }
}

int64_t TimerRNG::GetRandomNumber() {
    return this->RNGTimer;
}