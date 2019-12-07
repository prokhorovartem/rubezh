#pragma once

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "SafeQueue.h"

using namespace std;


class ThreadPool {
private:
  class ThreadWorker {
  private:
    int m_id;
    ThreadPool * m_pool;
  public:
    ThreadWorker(ThreadPool * pool, const int id)
      : m_pool(pool), m_id(id) {
    }

    void operator()() {
      function<void()> func;
      bool dequeued;
      while (!m_pool->m_shutdown) {
        {
          unique_lock<mutex> lock(m_pool->m_conditional_mutex);
          if (m_pool->m_queue.empty()) {
            m_pool->m_conditional_lock.wait(lock);
          }
          dequeued = m_pool->m_queue.dequeue(func);
        }
        if (dequeued) {
          func();
        }
      }
    }
  };

  bool m_shutdown;
  SafeQueue<function<void()>> m_queue;
  vector<thread> m_threads;
  mutex m_conditional_mutex;
  condition_variable m_conditional_lock;

public:
  explicit ThreadPool(const int n_threads)
    : m_threads(vector<thread>(n_threads)), m_shutdown(false) {
  }

  ThreadPool(const ThreadPool &) = delete;
  ThreadPool(ThreadPool &&) = delete;

  ThreadPool & operator=(const ThreadPool &) = delete;
  ThreadPool & operator=(ThreadPool &&) = delete;

  // Inits thread pool
  void init() {
    for (int i = 0; i < m_threads.size(); ++i) {
      m_threads[i] = thread(ThreadWorker(this, i));
    }
  }

  // Waits until threads finish their current task and shutdowns the pool
  void shutdown() {
    m_shutdown = true;
    m_conditional_lock.notify_all();
    
    for (auto & m_thread : m_threads) {
      if(m_thread.joinable()) {
        m_thread.join();
      }
    }
  }

  // Submit a function to be executed asynchronously by the pool
  template<typename F, typename...Args>
  auto submit(F&& f, Args&&... args) -> future<decltype(f(args...))> {
    // Create a function with bounded parameters ready to execute
    function<decltype(f(args...))()> func = bind(forward<F>(f), forward<Args>(args)...);
    // Encapsulate it into a shared ptr in order to be able to copy construct / assign 
    auto task_ptr = make_shared<packaged_task<decltype(f(args...))()>>(func);

    // Wrap packaged task into void function
    function<void()> wrapper_func = [task_ptr]() {
      (*task_ptr)(); 
    };

    // Enqueue generic wrapper function
    m_queue.enqueue(wrapper_func);

    // Wake up one thread if its waiting
    m_conditional_lock.notify_one();

    // Return future from promise
    return task_ptr->get_future();
  }
};
