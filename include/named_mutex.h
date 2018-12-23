#pragma once
#include <memory>

namespace tw
{
  class NamedMutex
  {
  public:
    enum TYPE
    {
     THREAD = 0,    // uses semaphore
     PROCESS_OWNER, // uses semaphore with shared memory + sem initialization
     PROCESS        // uses semaphore with shared memory, no sem initialization
    };

    NamedMutex(const char* name, enum TYPE);
    ~NamedMutex();

    NamedMutex(const NamedMutex&) = delete;
    NamedMutex(NamedMutex&&) = delete;
    NamedMutex& operator=(const NamedMutex&) = delete;
    NamedMutex& operator=(NamedMutex&&) = delete;

    int  Init();
    void Lock();
    void UnLock();

  private:
    class Impl;
    std::unique_ptr<Impl> pimpl;
  };

  std::shared_ptr<NamedMutex> CreateNamedMutex(const char* name, NamedMutex::TYPE shared);
}
