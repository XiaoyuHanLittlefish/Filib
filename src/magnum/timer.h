#ifndef MAGNUM_TIMER_H__
#define MAGNUM_TIMER_H__

#include <chrono>

/**
 * @brief 
 * 包含了用于计时的timer类
 */

class timer
{
public:
    using millisecond = int64_t;

public:
    timer();
    ~timer();
    void start();
    millisecond end();

private:
    bool running;
    millisecond timestamp;
};

timer::timer() : running(false), timestamp(0)
{
}

timer::~timer()
{
}

void timer::start()
{
    this->running = true;

    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());

    this->timestamp = (millisecond)ms.count();
}

timer::millisecond timer::end()
{
    if (!this->running) 
    {
        return -1;
    }

    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());

    return (millisecond)ms.count() - this->timestamp;
}

#endif //!MAGNUM_TIMER_H__