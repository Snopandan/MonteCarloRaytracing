#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <atomic>

#include "WorkerThread.h"
#include "WorkItem.h"
#include "WorkItemComparison.h"

class WorkerThread;

class ThreadPool {

public:
  ThreadPool(const unsigned int numberOfWorkers = std::thread::hardware_concurrency()-1);

  ~ThreadPool();

  void dig();

  void add(WorkItem* workItem);

  WorkItem* pop();

  void clearThreads();

  void clearWorkItems();

  void setNumberOfWorkers(const unsigned int numberOfWorkers);

  void wait();

  void workerFinsihedJob();

protected:

private:
  bool dig_;

  unsigned int numberOfWorkers_;
  unsigned int workerThreadsCounter_;

  unsigned int numberOfAddedWorkItems_;
  unsigned int numberOfFinishedWorkItems_;

  std::mutex queueLock_;
  std::mutex finishedLock_;
  std::priority_queue<WorkItem*, std::vector<WorkItem*>, WorkItemComparison> queue_;

  std::vector<WorkerThread*> workThreads_;

};



#endif // THREADPOOL_H