#include "ThreadPool.h"


ThreadPool::ThreadPool(const unsigned int numberOfWorkers) 
: numberOfWorkers_{numberOfWorkers}
, workerThreadsCounter_{0}
, dig_{false}
, numberOfAddedWorkItems_{0}
, numberOfFinishedWorkItems_{0}
{

}


ThreadPool::~ThreadPool() {
  clearThreads();

  for(unsigned int i=0; i<workThreads_.size(); i++) {
    delete workThreads_[i];
  }
}


void ThreadPool::clearThreads() {
  setNumberOfWorkers(0);
  dig_ = false;
}


void ThreadPool::clearWorkItems() {
  WorkItem* workItem = nullptr;
  std::lock_guard<std::mutex> guardian(queueLock_);
  while( !queue_.empty() ) {
    workItem = queue_.top();
    queue_.pop();
    delete workItem;
  }
}


void ThreadPool::setNumberOfWorkers(const unsigned int numberOfWorkers) {

  if( numberOfWorkers_ >  numberOfWorkers ) {
    const unsigned int numberOfWorkersToRemove = numberOfWorkers_ - numberOfWorkers;

    for(unsigned int i=0; i<numberOfWorkersToRemove; i++) {
      const unsigned int workThread = workThreads_.size()-1;
      workThreads_[workThread]->stop();
      workThreads_.erase(workThreads_.begin() + workThread);
    }

  } else if( numberOfWorkers_ <  numberOfWorkers ) {
    const unsigned int numberOfWorkersToAdd = numberOfWorkers - numberOfWorkers_;

    for(unsigned int i=0; i<numberOfWorkersToAdd; i++) {
      workThreads_.push_back(new WorkerThread{workerThreadsCounter_++, this});
      if( dig_ ) {
        workThreads_[workThreads_.size()-1]->run();
      }
    }
  }

   numberOfWorkers_ = numberOfWorkers;

  for(unsigned int i=0; i<workThreads_.size(); i++) {
    workThreads_[i]->stop();
  }

}


void ThreadPool::dig() {
  dig_ = true;

  for(unsigned int i=0; i<numberOfWorkers_; i++) {
    workThreads_.push_back(new WorkerThread{workerThreadsCounter_++, this});
    workThreads_[i]->run();
  }

}


void ThreadPool::add(WorkItem* workItem) {
  std::lock_guard<std::mutex> guardian(queueLock_);
  queue_.push(workItem);
  numberOfAddedWorkItems_++;
}


WorkItem* ThreadPool::pop() {
  WorkItem* workItem = nullptr;
  std::lock_guard<std::mutex> guardian(queueLock_);
  if( !queue_.empty() ) {
    workItem = queue_.top();
    queue_.pop();
  }
  return workItem;
}


void ThreadPool::wait() {
  bool wait = true;
  while( wait ) {
    finishedLock_.lock();
    if( numberOfFinishedWorkItems_ == numberOfAddedWorkItems_ ) {
      wait = false;
    }
    finishedLock_.unlock();

    if( wait ) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }
}


void ThreadPool::workerFinsihedJob() {
  std::lock_guard<std::mutex> guardian(finishedLock_);
  numberOfFinishedWorkItems_++;
}
