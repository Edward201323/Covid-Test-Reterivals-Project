#include "Timer.hpp"
#include <string>

Timer::Timer() {
    state = State::INVALID;
}

void Timer::start() {
    // To try to make the timing as accurate as possible,
    // we'll make sure setting the start time is the very LAST thing
    // this function does

    // Keep track of the fact that the timer is running,
    // so stop() and read() know whether to throw exceptions
    state = State::STARTED;

    startTime = getCurrentTime();
}

void Timer::stop() {
    // To try to make the timing as accurate as possible,
    // we'll make sure measuring the end time is the very FIRST thing
    // this function does, and only afterwards will we handle
    // the error-checking and whether to store the measurement.
    auto temp = getCurrentTime();

    if (state != State::STARTED) {
        // It doesn't make any sense to stop a timer that wasn't started.
        // We'll keep track of the fact that something weird has happened
        // so read() knows whether to throw exceptions,
        // and then we'll throw one of our own.
        state = State::INVALID;
        throw std::string("Timer::stop error: can't stop a Timer that hasn't been started");
    } else {
        // Keep track of the fact that the timer has ended,
        // so read() knows it's ok to try to return a measurement.
        state = State::ENDED;

        // store the ending time we measured earlier.
        endTime = temp;
    }
}

int Timer::read() const {
    if (state != State::ENDED) {
        throw std::string("Timer::read error: can't read a Timer that has not been started and then stopped");
    } else {
        auto measurement = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        return measurement.count();
    }
}

std::chrono::high_resolution_clock::time_point Timer::getCurrentTime() const {
    return std::chrono::high_resolution_clock::now();
}