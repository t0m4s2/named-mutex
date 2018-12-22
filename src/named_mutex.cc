#include "named_mutex.h"
#include "named_mutex_impl.h"

namespace tw
{
  NamedMutex* CreateNamedMutex(const char* name, bool shared)
  {
      return new NamedMutex(name, shared);
  }

  NamedMutex::NamedMutex(const char* name, bool shared) : pimpl{std::make_unique<NamedMutexImpl>(name, shared)}
  {
  }

  NamedMutex::~NamedMutex() = default;

  void NamedMutex::Lock()
  {
      pimpl->Lock();
  }

  void NamedMutex::UnLock()
  {
      pimpl->UnLock();
  }
}
