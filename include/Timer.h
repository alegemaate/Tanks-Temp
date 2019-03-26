/*
 * Timer.h
 * A simple, cross platform timer class
 *  based on chrono
 * Allan Legemaate
 * 25-03-2019
 */

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

using namespace std::chrono;

// Timer class
class Timer {
  public:
    Timer() {};
    virtual ~Timer() {};

    // Start time
    void Start();

    // Reset timer
    void Reset();

    // Get ms since started
    template <typename Precision>
    double GetElapsedTime();

  private:
    // Holds time points for start and end
    time_point<high_resolution_clock> t1, t2;
};

#endif // TIMER_H
