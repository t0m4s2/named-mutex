#pragma once
#include <memory>

namespace tw
{
  class NamedMutex
  {
  public:
    NamedMutex(const char* name, bool shared);
    ~NamedMutex();

    NamedMutex& operator=(const NamedMutex&) = delete;
    NamedMutex(const NamedMutex&) = delete;

    void Lock();
    void UnLock();

  private:
    class NamedMutexImpl;
    std::unique_ptr<NamedMutexImpl> pimpl;
  };

  NamedMutex* CreateNamedMutex(const char* name, bool shared);
}
