#pragma once

#include <chrono>

// A class that measures the real-world time (or something close enough to it)
// that has passed between when its `start` and `stop` functions have been called.
// It only stores a single, most-recent measurement.
class Timer {
private:
    // When the timer started
    std::chrono::high_resolution_clock::time_point startTime;

    // When the timer ended
    std::chrono::high_resolution_clock::time_point endTime;

    // A helper method that gets the current system time.
    std::chrono::high_resolution_clock::time_point getCurrentTime() const;

    // An enum that will help us keep track of whether they're using the Timer correctly
    enum class State {
        INVALID, // they haven't started the timer yet, or they did something weird
        STARTED, // they have started the timer but have not yet stopped it
        ENDED, // they ended the timer, so read() should be allowed
    } state; // We can declare a variable of this type
             // in the same statement as where we declare the type.
             // Neat. (Note: I usually don't write code this way
             // because I think it's an easy detail to miss)

public:
    Timer();

    // Starts counting how much time is passing.
    // This will clear any stored data from previous uses of the same Timer object,
    // or reset the start time if you've already started timing something.
    // You will not be able to `read()` the Timer until you run `stop()`.
    void start();

    // Stops counting how much time is passing.
    // Invalidates any existing measurement and throws an exception
    // if you haven't called `start` before calling this method.
    void stop();

    // Returns the amount of time most recently measured by the timer, in milliseconds:
    // specifically, the amount of time between the most recent calls to `start` and then `stop`.
    // Throws an exception if you have not done so.
    int read() const;
};