#include <semaphore.h>
#include "named_mutex.h"

namespace tw
{

  class NamedMutex::NamedMutexImpl
  {
  public:
    NamedMutexImpl(const char* name, bool shared);
    ~NamedMutexImpl();

    NamedMutexImpl& operator=(const NamedMutexImpl&) = delete;
    NamedMutexImpl(const NamedMutexImpl&) = delete;

    int  Init();
    void Lock();
    void UnLock();

  private:
    sem_t *semaphore_ = nullptr;
    std::string name_;          // mutex name
    bool shared_ = false;       // is shared between processes
  };
}
