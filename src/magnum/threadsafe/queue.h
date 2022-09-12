#ifndef MAGNUM_THREADSAFE_QUEUE_H__
#define MAGNUM_THREADSAFE_QUEUE_H__

#include <mutex>
#include <queue>
#include <condition_variable>

namespace threadsafe
{

    template <class T>
    class Queue
    {
    public:
        void push(T new_value)
        {
            // std::lock_guard<std::mutex> lk(m_mtx);
            // m_queue.push(new_value);
            // m_cond.notify_one();

            auto data = std::make_shared(std::move(new_value));
            std::lock_guard<std::mutex> lk(m_mtx);
            m_queue.push(data);
            m_cond.notify_one();
        }

        void wait_pop(T &value)
        {
            std::unique_lock<std::mutex> lk(m_mtx);
            m_cond.wait(lk, [this]
                        { return !m_queue.empty(); });
            // value = std::move(m_queue.front());
            value = std::move(*m_queue.front());
            m_queue.pop();
        }

        std::shared_ptr<T> wait_pop()
        {
            std::unique_lock<std::mutex> lk(m_mtx);
            m_cond.wait(lk, [this]
                        { return !m_queue.empty(); });
            // std::shared_ptr<T> res(
            //     std::make_shared<T>(std::move(m_queue.front())));
            std::shared_ptr<T> res = m_queue.front();
            m_queue.pop();
            return res;
        }

        bool try_pop(T &value)
        {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (m_queue.empty())
            {
                return false;
            }
            // value = std::move(m_queue.front());
            value = std::move(*m_queue.front());
            m_queue.pop();
            return true;
        }

        std::shared_ptr<T> try_pop()
        {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (m_queue.empty())
            {
                return std::shared_ptr<T>();
            }
            // std::shared_ptr<T> res(
            //     std::make_shared(std::move(m_queue.front())));
            std::shared_ptr<T> res = m_queue.front();
            m_queue.pop();
            return res;
        }

        bool empty() const
        {
            std::lock_guard<std::mutex> lk(m_mtx);
            return m_queue.empty();
        }

    private:
        mutable std::mutex m_mtx;
        std::queue<std::shared_ptr<T>> m_queue;
        std::condition_variable m_cond;
    };

    template <class T>
    class queue
    {
    public:
        queue() : m_head(new node), m_tail(m_head.get()){};

        queue(const queue &other) = delete;

        queue &operator=(const queue &other) = delete;

        std::shared_ptr<T> try_pop()
        {
            std::unique_ptr<node> old_head = pop_head();
            return old_head ? old_head->data_ : std::shared_ptr<T>();
        }

        void push(T new_value)
        {
            std::shared_ptr<T> new_data(
                std::make_shared<T>(std::move(new_value)));
            std::unique_ptr<node> p(new node);
            node *const new_tail = p.get();

            std::lock_guard<std::mutex> tail_lock(m_tail_mtx);
            m_tail->data_ = new_data;
            m_tail->next_ = std::move(p);
            m_tail = new_tail;
        }

    private:
        struct node
        {
            std::shared_ptr<T> data_;
            std::unique_ptr<node> next_; // 自动析构，但数据量较大时会爆栈，需要修改
        };

        std::mutex m_head_mtx;
        std::unique_ptr<node> m_head;
        std::mutex m_tail_mtx;
        node *m_tail;

    private:
        node *get_tail()
        {
            std::lock_guard<std::mutex> tail_lock(m_tail_mtx);
            return m_tail;
        }

        std::unique_ptr<node> pop_head()
        {
            std::lock_guard<std::mutex> head_lock(m_head_mtx);
            if (m_head.get() == get_tail())
            {
                return std::unique_ptr<node>(nullptr);
            }
            std::unique_ptr<node> old_head = std::move(m_head);
            m_head = std::move(old_head->next_);
            return old_head;
        }
    };

} // namespace threadsafe

#endif //! MAGNUM_THREADSAFE_QUEUE_H__