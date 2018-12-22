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

    int  Init();
    void Lock();
    void UnLock();

  private:
    class NamedMutexImpl;
    std::unique_ptr<NamedMutexImpl> pimpl;
  };

  std::shared_ptr<NamedMutex> CreateNamedMutex(const char* name, bool shared);
}
