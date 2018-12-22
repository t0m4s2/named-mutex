#include "named_mutex.h"
#include "named_mutex_impl.h"

namespace tw
{
  std::shared_ptr<NamedMutex> CreateNamedMutex(const char* name, bool shared)
  {
      return std::make_shared<NamedMutex>(name, shared);
  }

  NamedMutex::NamedMutex(const char* name, bool shared) : pimpl{std::make_unique<NamedMutexImpl>(name, shared)}
  {
  }

  NamedMutex::~NamedMutex() = default;

  int NamedMutex::Init()
  {
      pimpl->Init();
  }

  void NamedMutex::Lock()
  {
      pimpl->Lock();
  }

  void NamedMutex::UnLock()
  {
      pimpl->UnLock();
  }
}
