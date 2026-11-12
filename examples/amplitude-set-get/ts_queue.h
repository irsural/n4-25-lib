#include <condition_variable>
#include <mutex>
#include <queue>

template<typename T>
class ts_queue_t
{
private:
  std::queue<T> m_queue;
  std::mutex m_mutex;
  std::condition_variable m_cv;

public:
  void push(T item)
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.push(item);
    m_cv.notify_one();
  }

  T pop()
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock, [this]() { return !m_queue.empty(); });
    T item = m_queue.front();
    m_queue.pop();
    return item;
  }

  bool empty()
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_queue.empty();
  }
};
