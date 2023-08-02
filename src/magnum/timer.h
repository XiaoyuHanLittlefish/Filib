#pragma once

#include <functional>
#include <thread>
#include <atomic>

namespace timer {
class Timer {
public:
  Timer() : m_active(false), m_period(0), m_repeat(-1) {}
  Timer(int repeat) : m_active(false), m_period(0), m_repeat(repeat) {}
  ~Timer() { stop(); }

  template <typename F, typename... Args>
  void start(int millisecond, F &&func, Args &&...args) {
    if (m_active.load()) {
      return;
    }

    m_period = millisecond;
    m_func = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
    m_active.store(true);

    m_thread = std::thread([&]() {
      if (m_repeat < 0) {
        while (m_active.load()) {
          std::this_thread::sleep_for(std::chrono::milliseconds(m_period));

          if (!m_active.load()) {
            return;
          }
          m_func();
        }
      } else {
        while (m_repeat > 0) {
          if (!m_active.load()) {
            return;
          }
          std::this_thread::sleep_for(std::chrono::milliseconds(m_period));

          if (!m_active.load()) {
            return;
          }
          m_func();

          --m_repeat;
        }
      }
    });

    m_thread.detach();
  }

  void stop() { m_active.store(false); }

private:
  std::thread m_thread;
  std::atomic<bool> m_active;
  std::function<void()> m_func;
  int m_period; // millisecond
  int m_repeat; // 重复次数，-1表示无限次
};
} // namespace timer