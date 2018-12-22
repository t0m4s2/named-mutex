#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include "named_mutex_impl.h"

namespace tw
{
  NamedMutex::NamedMutexImpl::NamedMutexImpl(const char* name, bool shared)
  {
    if(name)
    {
      sem_name_ = name;
      semaphore_ = sem_open("test", O_CREAT, S_IRWXU, 1);
    }
    else
    {
      semaphore_ = (sem_t*)malloc(sizeof(sem_t));
      sem_init(semaphore_, 0, 1);
    }

    if(semaphore_ == SEM_FAILED)
    {
      printf("FAILED\n");
      semaphore_ = nullptr;
    }
  }


  NamedMutex::NamedMutexImpl::~NamedMutexImpl()
  {
    sem_close(semaphore_);
    sem_unlink("test");
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
