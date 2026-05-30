#ifndef THREADPOOL_LIBRARY_HPP
#define THREADPOOL_LIBRARY_HPP

#include <cstdint>
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <windows.h>//NOLINT(misc-include-cleaner)
#include <mutex>
#include <condition_variable>
#include <atomic>

struct RayTraceWorkItem
{
  int y;
  int x;
  RayTraceWorkItem(int x, int y) : y(y), x(x) {}
};

using WorkerFct = std::function<void(const std::shared_ptr<RayTraceWorkItem>& workItem)>;


class Threadpool
{
private:
  std::vector<std::thread> threads;
  std::queue<std::shared_ptr<RayTraceWorkItem>> workItems;
  WorkerFct workFunction;
  std::mutex workMutex;
  std::condition_variable workCv;
  std::atomic<bool> stop{false};

  void thread(unsigned int cpuToPinTo)
  {
    #ifdef _WINDOWS
    SetThreadAffinityMask(GetCurrentThread(), cpuToPinTo);// NOLINT(misc-include-cleaner)
    #else
    // Create a cpu_set_t object representing a set of CPUs. Clear it and mark
    // only CPU i as set.
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpuToPinTo, &cpuset);
    int rc = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    if (rc != 0) { std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n"; }
    #endif

    while (!stop.load()) {
      std::shared_ptr<RayTraceWorkItem> item;
      {
        std::unique_lock<std::mutex> lk(workMutex);
        workCv.wait(lk, [&]{ return stop.load() || !workItems.empty(); });
        if (stop.load() && workItems.empty()) break;
        item = workItems.front();
        workItems.pop();
      }
      if (item) workFunction(item);
    }
  }

public:
  Threadpool(unsigned int numThreads, WorkerFct _workFunction) : workFunction(_workFunction)
  {
    for (unsigned int i = 0; i < numThreads; ++i)
      threads.emplace_back(&Threadpool::thread, this, i + 1);
  }
  explicit Threadpool(WorkerFct _workFunction) : Threadpool(std::thread::hardware_concurrency(), _workFunction){ }

  ~Threadpool() {
    stop.store(true);
    workCv.notify_all();
    for (auto &t : threads) if (t.joinable()) t.join();
  }

  void addWorkItem(std::shared_ptr<RayTraceWorkItem> workItem) {
    {
      std::lock_guard<std::mutex> lk(workMutex);
      workItems.push(std::move(workItem));
    }
    workCv.notify_one();
  }

  bool hasWork() {
    std::lock_guard<std::mutex> lk(workMutex);
    return !workItems.empty();
  }
};

#endif