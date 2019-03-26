#include "Timer.h"

// Available modes, fit to needs
template double Timer::GetElapsedTime<nanoseconds>();
template double Timer::GetElapsedTime<microseconds>();
template double Timer::GetElapsedTime<milliseconds>();
template double Timer::GetElapsedTime<seconds>();
template double Timer::GetElapsedTime<minutes>();
template double Timer::GetElapsedTime<hours>();

// Start Timer
void Timer::Start() {
  t1 = high_resolution_clock::now();
}

// Reset (just calls start)
void Timer::Reset() {
  Start();
}

// Get ms since started
template <typename Precision>
double Timer::GetElapsedTime() {
  // Get time now
  t2 = high_resolution_clock::now();

  // Choose precision
  auto time_diff = duration_cast<Precision>(t2 - t1);

  // Return time as double
  return time_diff.count();
}
