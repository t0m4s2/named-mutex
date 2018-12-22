//#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <iostream>
#include "named_mutex_impl.h"

namespace tw
{
  NamedMutex::NamedMutexImpl::NamedMutexImpl(const char* name, bool shared) : name_(name), shared_(shared)
  {
  }

  NamedMutex::NamedMutexImpl::~NamedMutexImpl()
  {
    if(name_.size())
    {
      sem_close(semaphore_);
      sem_unlink(name_.c_str());
    }
  }

  int NamedMutex::NamedMutexImpl::Init()
  {
    if(name_.size() != 0)
    {
      semaphore_ = sem_open(name_.c_str(), O_CREAT, S_IRWXU, 1);
      if (semaphore_ == SEM_FAILED)
      {
        std::cout << "Failed to open semaphore\n" << std::endl;
        return -1;
      }
    }
    else
    {
      std::cout << "Invalid mutex name" << std::endl;
      return -1;
    }

    return 0;
  }

  void NamedMutex::NamedMutexImpl::Lock()
  {
    if(semaphore_)
    {
      sem_wait(semaphore_);
    }
  }

  void NamedMutex::NamedMutexImpl::UnLock()
  {
    if(semaphore_)
    {
      sem_post(semaphore_);
    }
  }
}
