#pragma once

#include <functional>
#include <thread>

namespace timer {
class Timer {
public:
  Timer() : m_active(false), m_period(0), m_repeat(-1) {}
  Timer(int repeat) : m_active(false), m_period(0), m_repeat(repeat) {}
  ~Timer() {}

  template <typename F, typename... Args>
  void start(int millisecond, F &&func, Args &&...args);

  void stop();

private:
  std::thread m_thread;
  std::atomic<bool> m_active;
  std::function<void()> m_func;
  int m_period; // millisecond
  int m_repeat;
};
} // namespace timer