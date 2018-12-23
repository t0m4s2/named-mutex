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
    class Impl;
    std::unique_ptr<Impl> pimpl;
  };

  std::shared_ptr<NamedMutex> CreateNamedMutex(const char* name, bool shared);
}
